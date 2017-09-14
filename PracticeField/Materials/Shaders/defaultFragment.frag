#version 330 core

in vec2 UV;
in vec3 Position_worldspace;
in vec3 Normal_cameraspace;
in vec3 ViewDirection_cameraspace;
in vec3 LightDirection_cameraspace;

out vec3 color;

uniform mat4 MV;

struct DirectionalLight{
	vec3 direction;
	vec3 color;
	float power;
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
uniform sampler2D texture_diffuse0;
uniform sampler2D texture_diffuse1;

uniform int texCountSpec;
uniform sampler2D texture_specular0;
uniform sampler2D texture_specular1;

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



void main(){
	MaterialColor matColor;

	if(texCountDiff == 0){
		matColor.diffuseColor = vec3(1.0);
		matColor.ambientColor = vec3(0.1);
		matColor.specularColor = vec3(1.0);
	}else{
		matColor.diffuseColor = texture( texture_diffuse0, UV ).rgb;
		matColor.ambientColor = vec3(0.1,0.1,0.1) * matColor.diffuseColor;
		matColor.specularColor = texture( texture_specular0, UV ).rgb;
	}

	vec3 viewDir = normalize(ViewDirection_cameraspace);
	vec3 normal = normalize(Normal_cameraspace);

	vec3 resultColor = vec3(0.0);

	resultColor += CalcDirLight(directionalLight0, matColor, normal, viewDir);

	//resultColor += CalcPointLight(pointLight0, matColor, normal, viewDir);	;
	color = resultColor;
}