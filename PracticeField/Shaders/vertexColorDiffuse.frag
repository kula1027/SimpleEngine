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

	float diff = max( dot( normal_,lightDir ), 0) + 0.3;
	vec3 reflectDir = reflect(-lightDir, normal_);
	float spec = pow(max(dot(viewDir_, reflectDir), 0.0), 32);

	vec3 ambient = matColor_.ambientColor;
	vec3 diffuse = directionalLight0.color * directionalLight0.power * diff * matColor_.diffuseColor;
	vec3 specular = vec3(0.3) * directionalLight0.color  * spec;	

	return diffuse;//(ambient + (diffuse + specular));
}

void main(){
	MaterialColor matColor;

	matColor.diffuseColor = vec3(0.5);
	matColor.ambientColor = vec3(0.5);
	matColor.specularColor = vec3(0.3);
	
	vec3 viewDir = normalize(frag_in.viewDirection_cameraSpace);
	vec3 normal = normalize(frag_in.normal_cameraSpace);

	vec3 resultColor = vec3(0.0);

	resultColor += CalcDirLight(directionalLight0, matColor, normal, viewDir);
	vec3 fColor = vec3(1.0f);
	if(frag_in.color.x <= 0.99f){
		fColor = vec3(0.4);
	}
	resultColor *= fColor;
	

	out_color = vec4(resultColor, 1);	
}