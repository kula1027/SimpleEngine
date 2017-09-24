#pragma once

#define strNoname "noname"

#include "Transform.h"

#include <vector>
#include <string>
#include <glm/gtc/matrix_transform.hpp>

class Renderer;
class BaseLight;
class Shader;
class Camera;
class IComponent;

class GameObject {
private:	
	Renderer* renderer;
	std::vector<IComponent*> components;

public:
	std::string name;
	Transform* transform;

	GameObject();

	void SetRenderer(Renderer* renderer_);
	Renderer* GetRenderer();
	void SetShader(Shader* shader_);	

	~GameObject();
};