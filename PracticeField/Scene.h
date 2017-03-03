#pragma once
#include "ObjectPool.h"
#include "Camera.h"

class Scene
{
private:
	GameWindow* currentWindow;

	ObjectPool* objectPool;
	Camera* camera;


public:
	Scene(GameWindow* gWindow_);
	~Scene();

	void Load();
	void UpdateObjects();
	void RenderObjects();	
};