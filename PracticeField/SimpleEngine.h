#pragma once

class Scene;

class SimpleEngine
{
private:
	Scene* currentScene;

public:
	~SimpleEngine();
	void Initialize(int width, int height, const char* name);
	void LoadScene(Scene* scene_);
	void LoadScene();
	void Begin();	
	void SetVsyncMode(bool flag);
};

