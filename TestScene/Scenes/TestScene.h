#pragma once
#include <Scene/Scene.h>

class TestScene : public Scene {
private:
	void ManyManyLights();

public:
	TestScene();
	~TestScene();

	void Load();
};

