#pragma once
#include "ObjectPool.h"
#include "Camera.h"

#include "PointLight.h"

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