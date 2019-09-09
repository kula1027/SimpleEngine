#pragma once

class Scene;

class SimpleEngine
{
private:
	static Scene* currentScene;

public:
	~SimpleEngine();
	static void Initialize(int width, int height, const char* name);
	static void LoadScene(Scene* scene_);
	static void LoadScene();
	static void Begin();
	static void SetVsyncMode(bool flag);

};

