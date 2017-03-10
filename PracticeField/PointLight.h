#pragma once

#include <iostream>

#include "Light.h"

class PointLight : public Light
{
private:		

public:
	PointLight();
	~PointLight();

	GLfloat range;

	void Update();
	void Use(Shader* shader);
};

