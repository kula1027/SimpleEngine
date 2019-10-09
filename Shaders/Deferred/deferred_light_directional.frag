#version 420 core

out vec4 FragColor;
  
in vec2 TexCoords;

uniform sampler2D gPosition;
uniform sampler2D gNormal;
uniform sampler2D gAlbedoSpec;

struct DirectionalLight{
	vec4 direction;					// 0, 16
	vec4 color;						// 16, 32
};

struct PointLight{
	vec4 position;
	vec4 color;
};

layout (std140) uniform LightData{
	int lightCountDirectional;				// 0, 4
	int lightCountPoint;					// 4, 8
	vec4 ambient;							// 16, 32
	DirectionalLight directionalLight[16];	// 32, 544		// 16 * 32 = 512
	PointLight pointLight[512];				// 544,	16928	// 512 * 32 = 16384
};

uniform vec3 viewPos;

void main()
{             
    // retrieve data from G-buffer
    vec3 fragPos = texture(gPosition, TexCoords).rgb;
    vec3 normal = texture(gNormal, TexCoords).rgb;
    vec3 Albedo = texture(gAlbedoSpec, TexCoords).rgb;
    float Specular = texture(gAlbedoSpec, TexCoords).a;
        
	
    vec3 lighting = texture(gAlbedoSpec, TexCoords).rgb * ambient.xyz;
	for(int loop = 0; loop < lightCountDirectional; loop++){
		vec3 lightDir = -directionalLight[loop].direction.xyz;
		lighting +=  max(dot(normal, lightDir), 0.0) * Albedo * directionalLight[loop].color.xyz;		   
	}
	

	FragColor = vec4(lighting, 1.0);	
	//FragColor = vec4(normal, 1);
}  