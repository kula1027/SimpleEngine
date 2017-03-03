#pragma once

#include "GameObject.h"
#include <vector>

class ObjectPool
{
private:
	std::vector<GameObject*> objects;
public:
	ObjectPool();	
	std::vector<GameObject*> GetObject();//TODO

	void AddObejct(GameObject* obj);
	void UpdateObjects();
	void RenderObjects(Camera* cam);

	~ObjectPool();
};

