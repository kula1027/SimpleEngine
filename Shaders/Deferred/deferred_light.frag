#version 420 core

out vec4 FragColor;
  
in vec2 TexCoords;

uniform sampler2D gPosition;
uniform sampler2D gNormal;
uniform sampler2D gAlbedoSpec;

//lightType
//0 directional
//1 point
//2 spot
/////////////////////////
struct LightObject {	
    vec4 position;					// 0, 16
	vec4 direction;					// 16, 32
    vec4 color;						// 32, 48
	int lightType;				// 48, 52    44??????
};

layout (std140) uniform LightData{
	vec4 ambient;					// 0, 16
	LightObject lightObject[512];	// 16, 26640		512 * 52 = 26624
	int lightCount;					// 26640, 26644
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
		lightDir = lightObject[loop].direction.xyz;
        diffuse = max(dot(normal, lightDir), 0.0) * Albedo * lightObject[loop].color.xyz;		
		diffuse = lightObject[loop].direction.xyz;
		break;

		case 1://point
        lightDir = normalize(lightObject[loop].position.xyz - fragPos);
        diffuse = max(dot(normal, lightDir), 0.0) * Albedo * lightObject[loop].color.xyz;
		
		break;

		case 2:
		diffuse = vec3(1, 0, 1);
		break;
		}

       
        lighting = vec3(1, 0, 1);
    }        

	FragColor = vec4(lighting, 1.0);	
	//FragColor = vec4(normal, 1);
}  