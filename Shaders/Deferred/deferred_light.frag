#version 420 core

out vec4 FragColor;
  
in vec2 TexCoords;

uniform sampler2D gPosition;
uniform sampler2D gNormal;
uniform sampler2D gAlbedoSpec;


struct LightData {
	int lightType;
    vec3 position;
    vec3 color;
};
const int NR_LIGHTS = 1;
uniform LightData lights[NR_LIGHTS];
uniform vec3 viewPos;

void main()
{             
    // retrieve data from G-buffer
    vec3 fragPos = texture(gPosition, TexCoords).rgb;
    vec3 normal = texture(gNormal, TexCoords).rgb;
    vec3 Albedo = texture(gAlbedoSpec, TexCoords).rgb;
    float Specular = texture(gAlbedoSpec, TexCoords).a;
        
    vec3 lighting = Albedo * 0.1; // hard-coded ambient component
    vec3 viewDir = normalize(viewPos - fragPos);
    for(int loop = 0; loop < NR_LIGHTS; loop++) {
		vec3 lightDir;
		vec3 diffuse;

		switch(lights[loop].lightType){
		case 0://directional
		lightDir = normalize(lights[loop].position - fragPos);
        diffuse = max(dot(normal, lightDir), 0.0) * Albedo * lights[loop].color;
		break;

		case 1://point
		 // diffuse
        lightDir = normalize(lights[loop].position - fragPos);
        diffuse = max(dot(normal, lightDir), 0.0) * Albedo * lights[loop].color;
		break;

		case 2:

		break;
		}

       
        lighting += diffuse;
    }
    
    FragColor = vec4(lighting, 1.0);	
	//FragColor = vec4(normal, 1);
}  