#version 330 core

out vec4 color;

uniform vec3 outlineColor;

void main(){
	color = vec4(outlineColor, 1.0);
}