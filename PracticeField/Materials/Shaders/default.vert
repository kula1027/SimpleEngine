#version 420 core

layout(location = 0) in vec3 attr_position;
layout(location = 1) in vec3 attr_normal;
layout(location = 2) in vec2 attr_texCoords;


out Vertex_Out{
	vec2 uv;
	vec3 position_worldSpace;
	vec3 normal_cameraSpace;
	vec3 viewDirection_cameraSpace;
	vec3 lightDirection_cameraSpace;
	vec4 fragPos_lightSpace;
}vertex_out;

uniform mat4 MVP;
uniform mat4 V;
uniform mat4 M;

struct DirectionalLight{	
	vec3 direction;
	vec3 color;
	float power;	
	sampler2D shadowMap;
	mat4 lightSpaceMatrix;
};
struct PointLight{
	vec3 position_worldSpace;	
	vec3 color;
	float power;
};

uniform PointLight pointLight0;
uniform DirectionalLight directionalLight0;

void main() {
	gl_Position = MVP * vec4(attr_position, 1);
	
	vertex_out.position_worldSpace = (M * vec4(attr_position, 1)).xyz;

	vec3 vertexPosition_cameraspace = (V * M * vec4(attr_position, 1)).xyz;
	vertex_out.viewDirection_cameraSpace = vec3(0, 0, 0) - vertexPosition_cameraspace;

	vec3 LightPosition_cameraspace = ( V * vec4(pointLight0.position_worldSpace, 1)).xyz;
	vertex_out.lightDirection_cameraSpace = LightPosition_cameraspace + vertex_out.viewDirection_cameraSpace;

	// Normal of the the vertex, in camera space
	vertex_out.normal_cameraSpace = (V * M * vec4(attr_normal, 0)).xyz; // Only correct if ModelMatrix does not scale the model ! Use its inverse transpose if not.
												
	vertex_out.fragPos_lightSpace = directionalLight0.lightSpaceMatrix * vec4(vertex_out.position_worldSpace, 1.0);	
																	
	vertex_out.uv = attr_texCoords;
}