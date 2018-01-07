#pragma once

#include <glm\glm.hpp>

using namespace glm;

class SimpleMath
{
public:
	static const float PI;

	SimpleMath();
	~SimpleMath();
	static float OrientedAngle(vec2 v);

	static float DegreeToRadian(float value);
	static float RadianToDegree(float value);
	static vec3 DegreeToRadianV3(vec3 value);
	static vec3 RadianToDegreeV3(vec3 value);
};

