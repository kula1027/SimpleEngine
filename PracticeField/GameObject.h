#pragma once

#define strNoname "noname"

#include <glm/gtc/matrix_transform.hpp>

#include "FileLoader.h"
#include "IUpdatable.h"
#include "Light.h"
#include "Transform.h"
#include "Camera.h"
#include "Renderer.h"
//Scene.h include하면 에러남, 저급한 C++...

class GameObject : public IUpdatable{
private:
	MeshModel* meshModel;
	Renderer* renderer;

public:
	string* name;
	Transform transform;

	GameObject();
	void Render(Camera* cam, std::vector<Light*> lights_);	

	void SetModel(MeshModel* meshModel_);
	void SetShader(Shader* shader_);

	void Update();
	~GameObject();
};