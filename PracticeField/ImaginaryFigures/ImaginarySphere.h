#pragma once

#include <glm\glm.hpp>
#include <vector>

using namespace glm;

class Mesh;
class Camera;
class BaseLight;

class ImaginarySphere
{
public:
	vec3 center;
	float radius = -1;

	ImaginarySphere();
	ImaginarySphere(vec3 center_, float radius_);
	~ImaginarySphere();


	static ImaginarySphere* GetBoundingSphere(Mesh* mesh, glm::vec3 center_);
};

