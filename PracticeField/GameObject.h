#pragma once

#include <vector>
#include <glm\glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "MaterialLoader.h"
#include "TextureLoader.h"
#include "ObjLoader.h"
#include "Renderer.h"
#include "Transform.h"
#include "Camera.h"

class GameObject
{
private:

public:
	GameObject();
	void Render(Camera* cam);
	Transform transform;
	Renderer* renderer;

	void Update();
	~GameObject();
};

