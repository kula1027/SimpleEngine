#pragma once

#include <glm\glm.hpp>
#include <string>

class Shader;

struct ShadowData {
	unsigned int resWidth = 1024;
	unsigned int resHeight = 1024;

	float nearPlane = 0.1f;
	float farPlane = 100.0f;
};

class BaseLight {
public:
	BaseLight();
	~BaseLight();

	float intensity;
	glm::vec3 color;
	glm::vec3 position;	

	std::string lightType;

	ShadowData shadowData;

	virtual inline void Use(Shader* shader) {}
};

