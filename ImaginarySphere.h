#pragma once

#include <glm\glm.hpp>
#include <vector>

using namespace glm;

class Mesh;
class Renderer;
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

	Renderer* renderer = NULL;
	void Render(Camera* cam, std::vector<BaseLight*> lights);

	static ImaginarySphere* GetBoundingSphere(Mesh* mesh, glm::vec3 center_);
};

