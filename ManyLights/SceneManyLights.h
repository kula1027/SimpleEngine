#pragma once
#include <Scene/Scene.h>

class SceneManyLights : public Scene{
private:
	void InstantiateLights();

public:
	SceneManyLights();
	~SceneManyLights();

	void Load() override;
};