#version 420 core

layout (location = 0) in vec3 attr_position;

uniform mat4 MVP;

void main() {		
	gl_Position = MVP * vec4(attr_position, 1.0f);	
}