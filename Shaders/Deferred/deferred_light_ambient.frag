#version 420 core

/////////////////////////
//lightType
//0 directional
//1 point
//2 spot
/////////////////////////
struct LightObject {	
	int lightType;					// 0, 4
    vec4 position;					// 16, 32
	vec4 direction;					// 32, 48
    vec4 color;						// 48, 64
};

layout (std140) uniform LightData{
	int lightCount;					// 0, 4
	vec4 ambient;					// 16, 32
	LightObject lightObject[512];	// 32, 32800		512 * 64 = 32768	
};


out vec4 FragColor;
  
in vec2 TexCoords;

uniform sampler2D gPosition;
uniform sampler2D gNormal;
uniform sampler2D gAlbedoSpec;

uniform vec3 viewPos;

void main()
{             
	FragColor = vec4(texture(gAlbedoSpec, TexCoords).rgb * ambient.xyz, 1.0);	
	//FragColor = vec4(normal, 1);
}  