#pragma once

#include <iostream>
#include <glm\glm.hpp>
#include <gl\glew.h>


class PointLight
{
private:		

public:
	PointLight();
	~PointLight();

	GLfloat range;
	glm::vec3 color;
	glm::vec3 position;
};

