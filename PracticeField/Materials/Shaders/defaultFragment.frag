#version 330 core

out vec4 color;

in vec2 UV;
in vec3 Position_worldspace;
in vec3 Normal_cameraspace;
in vec3 ViewDirection_cameraspace;
in vec3 LightDirection_cameraspace;
in vec4 FragPosLightSpace;

uniform mat4 MV;

struct DirectionalLight{	
	vec3 direction;
	vec3 color;
	float power;	
	sampler2D shadowMap;
	mat4 lightSpaceMatrix;
};

struct PointLight{
	vec3 position_worldspace;
	vec3 color;
	float power;
};

struct MaterialColor{
	vec3 diffuseColor; 
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
	float spec = pow(max(dot(viewDir_, reflectDir), 0.0), 32);

	vec3 ambient = matColor_.ambientColor;
	vec3 diffuse = directionalLight0.color * directionalLight0.power * diff * matColor_.diffuseColor;
	vec3 specular = vec3(0.3) * directionalLight0.color  * spec;	

	return (ambient + shadow_ * (diffuse + specular));
}

vec3 CalcPointLight(PointLight light_, MaterialColor matColor_, vec3 normal_, vec3 viewDir_){
	vec3 lightDir = normalize(LightDirection_cameraspace);

	float diff = max(dot(normal_, lightDir), 0.0);
	vec3 reflectDir = reflect(-lightDir, normal_);
	float spec = pow(max(dot(viewDir_, reflectDir), 0.0), 32);

	float distance = length(light_.position_worldspace - Position_worldspace );
	float attenuation = 1.0 / (distance * distance);

	vec3 ambient = matColor_.ambientColor;
	vec3 diffuse = pointLight0.color * pointLight0.power * diff * matColor_.diffuseColor;
	vec3 specular = pointLight0.color * spec * pointLight0.power * matColor_.specularColor;

	ambient *= attenuation;
	diffuse *= attenuation;
	specular *= attenuation;

	return (ambient + diffuse + specular);	
}

float CalcShadow(vec4 fragPosLightSpace_){
	// perform perspective divide
    vec3 projCoords = fragPosLightSpace_.xyz / fragPosLightSpace_.w;
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
		matColor.diffuseColor = vec3(1.0);
		matColor.ambientColor = vec3(0.1);
		matColor.specularColor = vec3(1.0);
	}else{
		matColor.diffuseColor = texture( texture_diffuse, UV ).rgb;
		matColor.ambientColor = vec3(0.1,0.1,0.1) * matColor.diffuseColor;
		matColor.specularColor = texture( texture_specular, UV ).rgb;		
	}

	vec3 viewDir = normalize(ViewDirection_cameraspace);
	vec3 normal = normalize(Normal_cameraspace);

	vec3 resultColor = vec3(0.0);

	float shadow = CalcShadow(FragPosLightSpace);

	resultColor += CalcDirLight(directionalLight0, matColor, normal, viewDir, shadow);
	//resultColor += CalcPointLight(pointLight0, matColor, normal, viewDir);
	
	
	color = vec4(resultColor, 1);
	//color = vec3( texture(directionalLight0.shadowMap, UV ).r);
}