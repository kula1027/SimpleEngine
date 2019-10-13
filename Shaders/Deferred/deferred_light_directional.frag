#version 420 core

out vec4 FragColor;

struct DirectionalLight{
	vec4 direction;					// 0, 16
	vec4 color;						// 16, 32
};

struct PointLight{
	float intensity;			// 0, 4
	vec4 position;				// 16, 32
	vec4 color;					// 32, 48
	vec4 attenK_c_l_q_range;			// 48, 64		//attenuation func Kc, Kl, Kq, Range
};

layout (std140) uniform LightData{
	int lightCountDirectional;				// 0, 4
	int lightCountPoint;					// 4, 8
	vec4 ambient;							// 16, 32
	DirectionalLight directionalLight[16];	// 32, 544		// 16 * 32 = 512
	PointLight pointLight[512];				// 544,	33312	// 512 * 64 = 32768
};

layout (std140) uniform CameraData{	
	mat4 V;					// 0, 64
	mat4 P;					// 64, 128
	mat4 VP;				// 128, 192
	vec4 cameraPosition;	// 192, 208
	vec4 cameraDirection;	// 208, 224
};
  
in vec2 TexCoords;

uniform sampler2D gPosition;
uniform sampler2D gNormal;
uniform sampler2D gAlbedoSpec;

void main()
{                 
    vec3 gFragPos = texture(gPosition, TexCoords).rgb;
    vec3 gNormal = texture(gNormal, TexCoords).rgb;
    vec3 gAlbedo = texture(gAlbedoSpec, TexCoords).rgb;
    float gSpecular = texture(gAlbedoSpec, TexCoords).a;
        
	
    vec3 lighting = texture(gAlbedoSpec, TexCoords).rgb * ambient.xyz;
	for(int loop = 0; loop < lightCountDirectional; loop++){
		//diffuse
		vec3 lightDir = -directionalLight[loop].direction.xyz;
		vec3 diffuse =  max(dot(gNormal, lightDir), 0.0) * gAlbedo * directionalLight[loop].color.xyz;		   

		// specular
		vec3 viewDir = normalize(cameraPosition.xyz - gFragPos);
		vec3 halfwayDir = normalize(lightDir + viewDir);          		
		float spec = pow(max(dot(gNormal, halfwayDir), 0.0), 32.0);
		vec3 specular = directionalLight[loop].color.xyz * spec;//* gSpecular

		lighting += diffuse + specular;
	}
	

	FragColor = vec4(lighting, 1.0);	
	//FragColor = vec4(normal, 1);
}  