#version 420 core

layout (location = 0) in vec2 attr_Pos;
layout (location = 1) in vec2 attr_TexCoords;

out vec2 TexCoords;

void main()
{
    TexCoords = attr_TexCoords;

    gl_Position = vec4(attr_Pos.x, attr_Pos.y, -1.0, 1.0); 
}  