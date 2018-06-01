#version 420 core

out vec4 out_color;

in Vertex_Out{
	in vec2 uv;
	in vec3 position_worldSpace;
	in vec3 normal_cameraSpace;
	in vec3 viewDirection_cameraSpace;
	in vec3 lightDirection_cameraSpace;
	in vec4 fragPos_lightSpace;
}frag_in;

uniform mat4 MV;

struct DirectionalLight{	
	vec3 direction;
	vec3 color;
	float power;	
	sampler2D shadowMap;
	mat4 lightSpaceMatrix;
};

struct PointLight{
	vec3 position_worldSpace;
	vec3 color;
	float power;
};

struct MaterialColor{
	vec4 diffuseColor; 
	vec3 ambientColor;
	vec3 specularColor;
};

uniform int texCountDiff;
uniform sampler2D texture_diffuse;

uniform int texCountSpec;
uniform sampler2D texture_specular;

#define NUM_POINTLIGHT 4
uniform int lightCountPoint;
uniform PointLight pointLight0;

uniform int lightCountDirectional;
uniform DirectionalLight directionalLight0;

vec3 CalcDirLight(DirectionalLight light_, MaterialColor matColor_, vec3 normal_, vec3 viewDir_, float shadow_){
	vec3 lightDir = normalize(-directionalLight0.direction);

	float diff = max( dot( normal_,lightDir ), 0);
	vec3 reflectDir = reflect(-lightDir, normal_);
	float spec = pow(max(dot(viewDir_, reflectDir), 0.0), 6);

	vec3 ambient = matColor_.ambientColor;
	vec3 diffuse = 0.8 * directionalLight0.color * directionalLight0.power * diff * matColor_.diffuseColor.rgb;
	vec3 specular = vec3(0.1) * directionalLight0.color  * spec;	

	return (ambient + shadow_ * (diffuse + specular));
}

vec3 CalcPointLight(PointLight light_, MaterialColor matColor_, vec3 normal_, vec3 viewDir_){
	vec3 lightDir = normalize(frag_in.lightDirection_cameraSpace);

	float diff = max(dot(normal_, lightDir), 0.0);
	vec3 reflectDir = reflect(-lightDir, normal_);
	float spec = pow(max(dot(viewDir_, reflectDir), 0.0), 32);

	float distance = length(light_.position_worldSpace - frag_in.position_worldSpace );
	float attenuation = 1.0 / (distance * distance);

	vec3 ambient = matColor_.ambientColor;
	vec3 diffuse = pointLight0.color * pointLight0.power * diff * matColor_.diffuseColor.rgb;
	vec3 specular = pointLight0.color * spec * pointLight0.power * matColor_.specularColor;

	ambient *= attenuation;
	diffuse *= attenuation;
	specular *= attenuation;

	return (ambient + diffuse + specular);	
}

float CalcShadow(vec4 fragPos_lightSpace_){//shadow = 0 -> shadowed
	// perform perspective divide
    vec3 projCoords = fragPos_lightSpace_.xyz / fragPos_lightSpace_.w;
    // transform to [0,1] range
    projCoords = projCoords * 0.5 + 0.5;
	if(projCoords.z > 1.0) return 1;
    // get closest depth value from light's perspective (using [0,1] range fragPosLight as coords)	
    float closestDepth = texture(directionalLight0.shadowMap, projCoords.xy).r; 

    // get depth of current fragment from light's perspective
    float currentDepth = projCoords.z;
    // check whether current frag pos is in shadow
    float shadow = currentDepth - 0.001 > closestDepth ? 0.0 : 1.0;

    return shadow;
}

void main(){
	MaterialColor matColor;

	if(texCountDiff == 0){
		matColor.diffuseColor = vec4(1.0);
		matColor.ambientColor = vec3(0.1);
		matColor.specularColor = vec3(1.0);
	}else{
		matColor.diffuseColor = texture( texture_diffuse, frag_in.uv ).rgba;
		matColor.ambientColor = vec3(0.3,0.3,0.3) * matColor.diffuseColor.rgb;
		matColor.specularColor = texture( texture_specular, frag_in.uv ).rgb;		
		if(matColor.diffuseColor.a < 0.05)discard;
	}

	vec3 viewDir = normalize(frag_in.viewDirection_cameraSpace);
	vec3 normal = normalize(frag_in.normal_cameraSpace);
	float shadow = 1;
	if(!gl_FrontFacing){
		viewDir = -viewDir;
		normal = -normal;		
	}else{
		shadow = CalcShadow(frag_in.fragPos_lightSpace);
	}
	vec3 resultColor = vec3(0.0);

	resultColor += CalcDirLight(directionalLight0, matColor, normal, viewDir, shadow);
	//resultColor += CalcPointLight(pointLight0, matColor, normal, viewDir);
	
	
	out_color = vec4(resultColor, 1.0);
	
}