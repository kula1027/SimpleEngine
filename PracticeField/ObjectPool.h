#pragma once

#include "IUpdatable.h"
#include "GameObject.h"
#include "Light.h"

class ObjectPool
{
private:
	std::vector<IUpdatable*> updatables;
	std::vector<GameObject*> gameObjects;
	std::vector<Light*> lights;

public:
	ObjectPool();	

	void AddGameObject(GameObject* obj);
	void AddUpdatable(IUpdatable* obj);
	void AddLight(Light* objLight);

	void UpdateObjects();
	void RenderObjects(Camera* cam);

	~ObjectPool();
};

