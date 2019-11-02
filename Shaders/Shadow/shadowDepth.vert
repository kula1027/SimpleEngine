#version 420 core
layout (location = 0) in vec3 pos;

uniform mat4 lightSpaceMatrix;
uniform mat4 M;

void main()
{
    gl_Position = lightSpaceMatrix * M * vec4(pos, 1.0);
}  