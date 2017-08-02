#pragma once

#include <glm\glm.hpp>
#include <gl\glew.h>

#include <iostream>

#include "IUpdatable.h"
#include "Shader.h"
#include "Time.h"

class Light: public IUpdatable {
public:
	Light();
	~Light();

	GLfloat intensity;
	glm::vec3 color;
	glm::vec3 position;	

	virtual inline void Use(Shader* shader) {}
};

