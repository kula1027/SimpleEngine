#pragma once

class Scene;

class SimpleEngine
{
private:
	Scene* currentScene;

	void UpdateObjects();
	void RenderObjects();

public:
	~SimpleEngine();
	void Initialize(int width, int height, const char* name);
	void Begin();	
	void SetVsyncMode(bool flag);
};

