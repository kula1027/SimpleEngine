#version 420 core

layout(location = 0) in vec3 attr_position;
layout(location = 1) in vec3 attr_normal;
layout(location = 2) in vec2 attr_texCoords;


layout (std140) uniform CameraData{	
	mat4 V;					// 0, 64
	mat4 P;					// 64, 128
	mat4 VP;				// 128, 192
	vec4 cameraPosition;	// 192, 208
};



uniform mat4 M;

out vec3 fragPos;
out vec3 normal;
out vec2 texCoords;

void main() {
	vec4 worldPos = M * vec4(attr_position, 1.0f);
	fragPos = worldPos.xyz;
	texCoords = attr_texCoords;

	mat3 normalMat = transpose(inverse(mat3(M)));
	normal = normalMat * attr_normal;

	gl_Position = VP * worldPos;
}