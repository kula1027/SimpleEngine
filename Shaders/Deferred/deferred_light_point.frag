#version 420 core

out vec4 FragColor;
  
in vec2 TexCoords;

uniform sampler2D gPosition;
uniform sampler2D gNormal;
uniform sampler2D gAlbedoSpec;

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

uniform vec3 viewPos;

void main()
{             
    // retrieve data from G-buffer
    vec3 fragPos = texture(gPosition, TexCoords).rgb;
    vec3 normal = texture(gNormal, TexCoords).rgb;
    vec3 Albedo = texture(gAlbedoSpec, TexCoords).rgb;
    float Specular = texture(gAlbedoSpec, TexCoords).a;
        
    vec3 lighting = Albedo * ambient.xyz;
    vec3 viewDir = normalize(viewPos - fragPos);
    for(int loop = 0; loop < lightCount; loop++) {
		vec3 lightDir;
		vec3 diffuse;		

		switch(lightObject[loop].lightType){
		case 0://directional
		lightDir = -lightObject[loop].direction.xyz;
        diffuse = max(dot(normal, lightDir), 0.0) * Albedo * lightObject[loop].color.xyz;				
		break;

		case 1://point
        lightDir = normalize(lightObject[loop].position.xyz - fragPos);
        diffuse = max(dot(normal, lightDir), 0.0) * Albedo * lightObject[loop].color.xyz;
		
		break;

		case 2:
		diffuse = vec3(1, 0, 1);
		break;
		}

       
        lighting += diffuse;
    }        

	FragColor = vec4(lighting, 1.0);	
	//FragColor = vec4(normal, 1);
}  