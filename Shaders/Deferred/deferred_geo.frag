#version 420 core

layout (location = 0) out vec3 gPosition;
layout (location = 1) out vec3 gNormal;
layout (location = 2) out vec4 gAlbedoSpec;

in vec3 fragPos;
in vec3 normal;
in vec2 texCoords;

uniform sampler2D texture_diffuse;
uniform sampler2D texture_specular;

void main()
{    
    //world space
    gPosition = fragPos;    
    gNormal = normalize(normal);
    
    gAlbedoSpec.rgb = texture(texture_diffuse, texCoords).rgb;	    
    gAlbedoSpec.a = texture(texture_specular, texCoords).r;
}