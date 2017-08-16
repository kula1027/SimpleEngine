#version 330 core

in vec2 UV;
in vec3 Position_worldspace;
in vec3 Normal_cameraspace;
in vec3 EyeDirection_cameraspace;
in vec3 LightDirection_cameraspace;

out vec3 color;

struct LightSource{
	vec3 position_worldspace;//direction if directional light
	vec3 color;
	float power;
};
struct MaterialColor{
	vec3 diffuseColor;
	vec3 ambientColor;
	vec3 specularColor;
};


uniform int difTexCount;
uniform sampler2D texture_diffuse0;
uniform sampler2D texture_diffuse1;
uniform sampler2D texture_specular0;
uniform sampler2D texture_specular1;
uniform mat4 MV;

uniform LightSource pointLight0;

void main(){
	MaterialColor matColor;
	matColor.diffuseColor = texture( texture_diffuse0, UV ).rgb;
	matColor.ambientColor = vec3(0.1,0.1,0.1) * matColor.diffuseColor;
	matColor.specularColor = texture( texture_specular0, UV ).rgb;
	
	// Distance to the light
	float distance = length( pointLight0.position_worldspace - Position_worldspace );

	// Normal of the computed fragment, in camera space
	vec3 n = normalize( Normal_cameraspace );
	// Direction of the light (from the fragment to the light)
	vec3 l = normalize( LightDirection_cameraspace );
	// Cosine of the angle between the normal and the light direction, 
	// clamped above 0
	//  - light is at the vertical of the triangle -> 1
	//  - light is perpendicular to the triangle -> 0
	//  - light is behind the triangle -> 0
	float cosTheta = clamp( dot( n,l ), 0,1 );
	
	// Eye vector (towards the camera)
	vec3 E = normalize(EyeDirection_cameraspace);
	// Direction in which the triangle reflects the light
	vec3 R = reflect(-l,n);
	// Cosine of the angle between the Eye vector and the Reflect vector,
	// clamped to 0
	//  - Looking into the reflection -> 1
	//  - Looking elsewhere -> < 1
	float cosAlpha = clamp(dot( E,R ), 0, 1);
	

	if(difTexCount == 0){
		color = 
			vec3(0.1,0.1,0.1) + 
			// Diffuse : "color" of the object
			pointLight0.color * pointLight0.power * cosTheta / (distance*distance) +
			// Specular : reflective highlight, like a mirror
			pointLight0.color * pointLight0.power * pow(cosAlpha,5) / (distance*distance);
	}else{
		color =
			// Ambient : simulates indirect lighting
			matColor.ambientColor +
			// Diffuse : "color" of the object
			matColor.diffuseColor * pointLight0.color * pointLight0.power * cosTheta / (distance*distance) +
			// Specular : reflective highlight, like a mirror
			matColor.specularColor * pointLight0.color * pointLight0.power * pow(cosAlpha,5) / (distance*distance);
	}
}
