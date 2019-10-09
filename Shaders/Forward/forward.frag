#version 420 core

out vec4 out_color;

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

	vec3 lightDir = normalize(-directionalLight[0].direction.xyz);  
    float diff = max(dot(normal, lightDir), 0.0);
    vec3 diffuse = directionalLight[0].color.xyz * diff * texture(texture_diffuse, frag_in.uv).rgb + ambient.xyz;  	

	out_color = vec4(diffuse, 1);	
	//out_color = vec4(lightDir, 1);
	//out_color = vec4(lightObject[0].color.w, 1);
}