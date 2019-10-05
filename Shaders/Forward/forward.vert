#version 420 core

layout(location = 0) in vec3 attr_position;
layout(location = 1) in vec3 attr_normal;
layout(location = 2) in vec2 attr_texCoords;


layout (std140) uniform CameraData{	
	mat4 V;			//0, 16
	mat4 P;			//16, 32
	mat4 VP;		//32, 48
};


out Vertex_Out{
	vec2 uv;
	vec3 position_worldSpace;
	vec3 normal_cameraSpace;
	vec3 viewDirection_cameraSpace;
	vec3 lightDirection_cameraSpace;
	vec4 fragPos_lightSpace;
}vertex_out;

uniform mat4 MVP;
uniform mat4 M;

void main() {
	gl_Position = MVP * vec4(attr_position, 1);
	
	vertex_out.position_worldSpace = (M * vec4(attr_position, 1)).xyz;

	vec3 vertexPosition_cameraspace = (V * M * vec4(attr_position, 1)).xyz;
	vertex_out.viewDirection_cameraSpace = -vertexPosition_cameraspace;
		
	// Normal of the the vertex, in camera space	
	vertex_out.normal_cameraSpace = (V * M * vec4(attr_normal, 0)).xyz; // Only correct if ModelMatrix does not scale the model ! Use its inverse transpose if not.
													
																	
	vertex_out.uv = attr_texCoords;
}