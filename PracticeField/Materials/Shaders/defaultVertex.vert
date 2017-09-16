#version 330 core

layout(location = 0) in vec3 position;
layout(location = 1) in vec3 normal;
layout(location = 2) in vec2 texCoords;

out vec2 UV;
out vec3 Position_worldspace;
out vec3 Normal_cameraspace;
out vec3 ViewDirection_cameraspace;
out vec3 LightDirection_cameraspace;
out vec4 FragPosLightSpace;

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
	vec3 position_worldspace;	
	vec3 color;
	float power;
};

uniform PointLight pointLight0;
uniform DirectionalLight directionalLight0;

void main() {
	
	gl_Position = MVP * vec4(position, 1);
	
	Position_worldspace = (M * vec4(position, 1)).xyz;

	vec3 vertexPosition_cameraspace = (V * M * vec4(position, 1)).xyz;
	ViewDirection_cameraspace = vec3(0, 0, 0) - vertexPosition_cameraspace;

	vec3 LightPosition_cameraspace = ( V * vec4(pointLight0.position_worldspace, 1)).xyz;
	LightDirection_cameraspace = LightPosition_cameraspace + ViewDirection_cameraspace;

	// Normal of the the vertex, in camera space
	Normal_cameraspace = (V * M * vec4(normal, 0)).xyz; // Only correct if ModelMatrix does not scale the model ! Use its inverse transpose if not.
												
	FragPosLightSpace = directionalLight0.lightSpaceMatrix * vec4(Position_worldspace, 1.0);	
																	
	UV = texCoords;
}