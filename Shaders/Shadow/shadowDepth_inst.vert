#version 420 core
layout(location = 0) in vec3 attr_position;
layout(location = 4) in mat4 attr_matModel;

uniform mat4 lightSpaceMatrix;

void main() {
    gl_Position = lightSpaceMatrix * attr_matModel * vec4(attr_position, 1.0);
}  