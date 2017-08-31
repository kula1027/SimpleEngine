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
	Transform transform;//TODO: ������������ �ٲ���ҵ�

	GameObject();

	void SetRenderer(Renderer* renderer_);

	void SetModel(MeshModel* meshModel_);
	MeshModel* GetModel();
	void SetShader(Shader* shader_);	

	~GameObject();
};