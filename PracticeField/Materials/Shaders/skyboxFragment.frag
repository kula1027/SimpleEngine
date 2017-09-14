#version 330 core
out vec4 color;

in vec3 TexCoords;

uniform samplerCube skyboxCubeMap;

void main()
{    
    color = texture(skyboxCubeMap, TexCoords);
}