#pragma once

#include <glm\glm.hpp>

using namespace glm;
//���̰� �ִ� ���� ���
class ImaginaryDisk
{
public:
	ImaginaryDisk();
	ImaginaryDisk(vec3 center_, float height_, float radius_);
	~ImaginaryDisk();

	float height;
	float radius;
	vec3 upCenter;
	vec3 downCenter;
	vec3 up;
};

