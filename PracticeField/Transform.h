#pragma once

#include <glm\glm.hpp>

class Transform
{
public:
	Transform();

	glm::vec3 position;
	glm::vec3 rotation;
	glm::vec3 scale;


	~Transform();
};

