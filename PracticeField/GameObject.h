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
class IComponent;

class GameObject {
private:	
	MeshModel* meshModel;
	Renderer* renderer;
	std::vector<IComponent*> components;

public:
	std::string name;
	Transform transform;//TODO: 포인터형으로 바꿔야할듯

	GameObject();

	void SetRenderer(Renderer* renderer_);

	void SetModel(MeshModel* meshModel_);
	MeshModel* GetModel();
	void SetShader(Shader* shader_);	

	~GameObject();
};