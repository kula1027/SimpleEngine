#pragma once

#include <glm\glm.hpp>
#include <gl\glew.h>

#include "IUpdatable.h"
#include "Shader.h"

class Light: public IUpdatable {
public:
	Light();
	~Light();

	GLfloat intensity;
	glm::vec3 color;
	glm::vec3 position;	

	virtual inline void Use(Shader* shader) {}
};

