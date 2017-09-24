#version 330 core
layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;
layout(location = 2) in vec2 texCoords;

uniform mat4 MVP;

out vec2 UV;

void main(){
    gl_Position = MVP * vec4(position, 1.0);
	UV = texCoords;
}  