#pragma once

#include <glm\glm.hpp>

using namespace glm;

class Calculator
{
public:
	static const float PI;

	Calculator();
	~Calculator();
	static float OrientedAngle(vec2 v);
};

