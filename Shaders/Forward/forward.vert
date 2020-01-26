#version 420 core

//! #include "../UniformBlock/ub_camera.glsl"
//! #include "../UniformBlock/ub_light.glsl"

layout(location = 0) in vec3 attr_position;
layout(location = 1) in vec3 attr_normal;
layout(location = 2) in vec2 attr_texCoords;

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