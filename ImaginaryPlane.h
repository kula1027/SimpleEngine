#pragma once
#include <glm\glm.hpp>

using namespace glm;
class ImaginaryPlane
{
public:
	vec3 normalVector;
	vec3 point;

	float d;//ax + by + cz + d = 0ÀÇ d

	ImaginaryPlane(vec3 normalVector_, vec3 point_);
	~ImaginaryPlane();
};

