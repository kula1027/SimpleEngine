#pragma once
#include <Scenes/Scene.h>

class TestScene : public Scene {
private:
	void ManyManyLights();

public:
	TestScene();
	~TestScene();

	void Load();
};

