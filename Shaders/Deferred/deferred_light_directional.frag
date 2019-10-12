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
	PointLight pointLight[512];				// 544,	25120	// 512 * 48 = 24576
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
		vec3 lightDir = -directionalLight[loop].direction.xyz;
		lighting +=  max(dot(gNormal, lightDir), 0.0) * gAlbedo * directionalLight[loop].color.xyz;		   
	}
	

	FragColor = vec4(lighting, 1.0);	
	//FragColor = vec4(normal, 1);
}  