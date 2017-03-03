#pragma once

#include "ObjectPool.h"
#include "GameWindow.h"
#include "Scene.h"

class SimpleEngine
{
private:
	GameWindow* gWindow;	

	Scene* currentScene;

	void UpdateObjects();
	void RenderObjects();

public:
	~SimpleEngine();
	void Initialize(int width, int height, const char* name);
	void Begin();	
	
};

