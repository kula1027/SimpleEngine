#version 420 core

layout(location = 0) in vec3 attr_position;
layout(location = 1) in vec3 attr_normal;
layout(location = 2) in vec2 attr_texCoords;


struct DirectionalLight{
	vec4 direction;					// 0, 16
	vec4 color;						// 16, 32
	mat4 lightVP;					// 32, 96
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
	DirectionalLight directionalLight[16];	// 32, 1568		// 16 * 96 = 1536
	PointLight pointLight[512];				// 1568, 34336	// 512 * 64 = 32768
};

layout (std140) uniform CameraData{	
	mat4 V;					// 0, 64
	mat4 P;					// 64, 128
	mat4 VP;				// 128, 192
	vec4 cameraPosition;	// 192, 208
	vec4 cameraDirection;	// 208, 224
	vec4 screenWidthHeight; // 224, 240
};


out V2F{
	vec2 uv;
	vec3 position_world;	
	vec3 normal_world;			
	vec4 position_light_directional[16];
}v2f;

uniform mat4 MVP;
uniform mat4 M;

void main() {
	gl_Position = MVP * vec4(attr_position, 1);
	
	v2f.uv = attr_texCoords;

	v2f.position_world = (M * vec4(attr_position, 1)).xyz;				
	v2f.normal_world = (M * vec4(attr_normal, 0)).xyz; 													
	for(int loop = 0; loop < lightCountDirectional; loop++){
		v2f.position_light_directional[loop] = directionalLight[loop].lightVP * vec4(v2f.position_world, 1);					
	}													
}