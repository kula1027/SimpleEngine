#version 420 core

layout(location = 0) in vec3 attr_position;
layout(location = 1) in vec3 attr_normal;
layout(location = 2) in vec2 attr_texCoords;

uniform mat4 MVP;
uniform mat4 M;

out vec2 out_texCoords;
out vec3 out_fragPos;
out vec3 out_normal;

void main() {
	gl_Position = MVP * vec4(attr_position, 1);
	
	out_texCoords = attr_texCoords;
	out_fragPos = (M * vec4(attr_position, 1.0)).xyz;
	out_normal = (M * vec4(attr_normal, 0)).xyz;
}