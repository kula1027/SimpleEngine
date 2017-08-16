#pragma once

#include <glm\glm.hpp>
#include <string>

class Shader;

class BaseLight {
public:
	BaseLight();
	~BaseLight();

	float intensity;
	glm::vec3 color;
	glm::vec3 position;	

	std::string lightType;

	virtual inline void Use(Shader* shader) {}
};

