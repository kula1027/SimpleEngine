#version 420 core
layout (location = 0) in vec3 attr_position;

uniform mat4 lightSpaceMatrix;
uniform mat4 M;

void main() {
    gl_Position = lightSpaceMatrix * M * vec4(attr_position, 1.0);
}  