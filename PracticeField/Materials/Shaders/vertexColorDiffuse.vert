#version 420 core

layout(location = 0) in vec3 attr_position;
layout(location = 1) in vec3 attr_normal;
layout(location = 3) in vec3 attr_color;


out Vertex_Out{
	vec3 normal_cameraSpace;
	vec3 viewDirection_cameraSpace;
	vec3 color;
}vertex_out;

uniform mat4 MVP;
uniform mat4 V;
uniform mat4 M;

struct DirectionalLight{	
	vec3 direction;
	vec3 color;
	float power;	
};

uniform DirectionalLight directionalLight0;

void main() {
	gl_Position = MVP * vec4(attr_position, 1);

	vec3 vertexPosition_cameraspace = (V * M * vec4(attr_position, 1)).xyz;
	vertex_out.viewDirection_cameraSpace = vec3(0, 0, 0) - vertexPosition_cameraspace;	
	vertex_out.normal_cameraSpace = (V * M * vec4(attr_normal, 0)).xyz; // Only correct if ModelMatrix does not scale the model ! Use its inverse transpose if not.
	vertex_out.color = attr_color;							
}