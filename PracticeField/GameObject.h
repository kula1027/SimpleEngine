#pragma once

#define strNoname "noname"

#include "Transform.h"

#include <vector>
#include <string>
#include <glm/gtc/matrix_transform.hpp>

class MeshModel;
class Renderer;
class BaseLight;
class Shader;
class Camera;

class GameObject {
private:
	MeshModel* meshModel;
	Renderer* renderer;

public:
	std::string name;
	Transform transform;

	GameObject();
	void Render(Camera* cam, std::vector<BaseLight*> lights_);

	void SetModel(MeshModel* meshModel_);
	void SetShader(Shader* shader_);

	void Update();
	~GameObject();
};