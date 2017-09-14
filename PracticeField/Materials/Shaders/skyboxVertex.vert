#version 330 core
layout (location = 0) in vec3 pos;

out vec3 TexCoords;

uniform mat4 P;
uniform mat4 V;

void main()
{
    TexCoords = pos;
    gl_Position = P * V * vec4(pos, 1.0);
}  