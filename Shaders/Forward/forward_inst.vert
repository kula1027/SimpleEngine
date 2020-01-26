#version 420 core
//! #include "../utils.glsl"
//! #include "../UniformBlock/ub_camera.glsl"
//! #include "../UniformBlock/ub_light.glsl"

layout(location = 0) in vec3 attr_position;
layout(location = 1) in vec3 attr_normal;
layout(location = 2) in vec2 attr_texCoords;
layout(location = 4) in mat4 attr_matModel;


out V2F{
	vec2 uv;
	vec3 position_world;	
	vec3 normal_world;			
	vec4 position_light_directional[16];
}v2f;

void main() {
	gl_Position = VP * attr_matModel * vec4(attr_position, 1);
	
	v2f.uv = attr_texCoords;

	v2f.position_world = (attr_matModel * vec4(attr_position, 1)).xyz;				
	v2f.normal_world = (attr_matModel * vec4(attr_normal, 0)).xyz; 													
	for(int loop = 0; loop < lightCountDirectional; loop++){
		v2f.position_light_directional[loop] = directionalLight[loop].lightVP * vec4(v2f.position_world, 1);					
	}													
}