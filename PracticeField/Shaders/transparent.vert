#version 330 core

layout(location = 0) in vec3 attr_position;
layout(location = 1) in vec3 attr_normal;
layout(location = 2) in vec2 attr_texCoords;

uniform mat4 MVP;

out vec2 UV;

void main(){
    gl_Position = MVP * vec4(attr_position, 1.0);
	UV = attr_texCoords;
}  