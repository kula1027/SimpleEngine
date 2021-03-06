#version 420 core

out vec4 out_color;

in Vertex_Out{
	vec3 normal_cameraSpace;
	vec3 viewDirection_cameraSpace;
	vec3 color;
}frag_in;

struct DirectionalLight{	
	vec3 direction;
	vec3 color;
	float power;	
};

struct MaterialColor{
	vec3 diffuseColor; 
	vec3 ambientColor;
	vec3 specularColor;
};

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

	matColor.diffuseColor = vec3(1);
	matColor.ambientColor = vec3(0.2);
	matColor.specularColor = vec3(1.0);
	
	vec3 viewDir = normalize(frag_in.viewDirection_cameraSpace);
	vec3 normal = normalize(frag_in.normal_cameraSpace);

	vec3 resultColor = vec3(0.0);

	resultColor += CalcDirLight(directionalLight0, matColor, normal, viewDir);
	float fColor = frag_in.color.x;
//	if(frag_in.color.x <= 0.999f){
//		fColor = 0.7f;
//	}
	resultColor.y *= fColor;
	resultColor.x *= fColor;
	

	out_color = vec4(resultColor, 1);	
}