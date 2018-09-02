#version 420 core

out vec4 out_color;

in Vertex_Out{
	vec2 uv;
	vec3 position_worldSpace;
	vec3 normal_cameraSpace;
	vec3 viewDirection_cameraSpace;
	vec3 lightDirection_cameraSpace;
	vec4 fragPos_lightSpace;
	vec3 layerColor;
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
	vec3 diffuseColor; 
	vec3 ambientColor;
	vec3 specularColor;
};

in vec3 refViewPos_worldSpace;
in vec3 normal_worldSpace;

uniform int texCountDiff;
uniform sampler2D texture_diffuse;

uniform int texCountSpec;
uniform sampler2D texture_specular;

#define NUM_POINTLIGHT 4
uniform int lightCountPoint;
uniform PointLight pointLight0;

uniform int lightCountDirectional;
uniform DirectionalLight directionalLight0;

vec3 CalcDirLight(DirectionalLight light_, MaterialColor matColor_, vec3 normal_, vec3 viewDir_){
	vec3 lightDir = normalize(-directionalLight0.direction);

	float diff = max( dot( normal_,lightDir ), 0);
	vec3 reflectDir = reflect(-lightDir, normal_);
	float spec = pow(max(dot(viewDir_, reflectDir), 0.0), 32);

	vec3 ambient = matColor_.ambientColor;
	vec3 diffuse = directionalLight0.color * directionalLight0.power * diff * matColor_.diffuseColor;
	vec3 specular = vec3(0.3) * directionalLight0.color  * spec;	

	return (ambient + diffuse + specular);
}

void main(){
	MaterialColor matColor;

	if(texCountDiff == 0){
		matColor.diffuseColor = vec3(1);
		matColor.ambientColor = vec3(0.2);
		matColor.specularColor = vec3(1.0);
	}

	vec3 viewDir = normalize(frag_in.viewDirection_cameraSpace);
	vec3 normal = normalize(frag_in.normal_cameraSpace);

	vec3 resultColor = vec3(0.0);

	resultColor += CalcDirLight(directionalLight0, matColor, normal, viewDir);

	float fColor = 1.0f;
	if(dot(normal_worldSpace, refViewPos_worldSpace) < 0){
		fColor = 0.7f;
	}
	
	resultColor.y *= fColor;
	resultColor.x *= fColor;
	
	out_color = vec4(resultColor, 1);
}