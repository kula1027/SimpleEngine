#pragma once
#include <Scene/Scene.h>

class TestScene : public Scene {

public:
	TestScene();
	~TestScene();

	void Load() override;
};