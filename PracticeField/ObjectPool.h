#pragma once

#include <vector>

class IUpdatable;
class GameObject;
class BaseLight;
class Camera;

class ObjectPool
{
private:
	std::vector<IUpdatable*> updatables;
	std::vector<GameObject*> gameObjects;
	std::vector<BaseLight*> lights;

public:
	ObjectPool();	

	void AddGameObject(GameObject* obj);
	void AddUpdatable(IUpdatable* obj);
	void AddLight(BaseLight* objLight);

	void UpdateObjects();
	void RenderObjects(Camera* cam);

	~ObjectPool();
};

