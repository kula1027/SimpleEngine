#pragma once

#include <glm\glm.hpp>

using namespace glm;

class Mesh;

class ImaginaryCube
{
public:
	vec3 center;
	vec3 max;
	vec3 min;

	ImaginaryCube();
	ImaginaryCube(vec3 center_, vec3 max_, vec3 min_);
	~ImaginaryCube();


	static ImaginaryCube* GetBoundingBox(Mesh* mesh);
};

