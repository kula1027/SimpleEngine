#version 420 core

out vec4 out_color;

/////////////////////////
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

in Vertex_Out{
	vec2 uv;
	vec3 position_worldSpace;
	vec3 normal_cameraSpace;
	vec3 viewDirection_cameraSpace;
	vec3 lightDirection_cameraSpace;
	vec4 fragPos_lightSpace;
}frag_in;

uniform sampler2D texture_diffuse;
uniform sampler2D texture_specular;

void main(){			
	vec3 viewDir = normalize(frag_in.viewDirection_cameraSpace);
	vec3 normal = normalize(frag_in.normal_cameraSpace);	

	vec3 lightDir = normalize(-lightObject[0].direction.xyz);  
    float diff = max(dot(normal, lightDir), 0.0);
    vec3 diffuse = lightObject[0].color.xyz * diff * texture(texture_diffuse, frag_in.uv).rgb * ambient.xyz;  	

	out_color = vec4(diffuse, 1);
	//out_color = vec4(diff, 0, 0, 1);
	//out_color = vec4(lightObject[0].color.w, 1);
}