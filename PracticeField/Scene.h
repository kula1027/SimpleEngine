#pragma once

#include "ObjectPool.h"
#include "Camera.h"

#include <string>
#include <vector>

class MeshModel;
class Shader;

class Scene
{
private:
	ObjectPool objectPool;
	Camera camera;

public:
	Scene();
	~Scene();

	void Load();
		
	void UpdateObjects();
	void RenderObjects();	
};