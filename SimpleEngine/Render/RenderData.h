#pragma once

#include <vector>
#include <glm\glm.hpp>

class Camera;
class BaseLight;

class RenderData {
public:
	RenderData();
	~RenderData();

	Camera* camera;
	std::vector<BaseLight*>* lights;	
};