#pragma once

#include <vector>

class IUpdatable;
class GameObject;
class BaseLight;
class Renderer;
class Camera;

class ObjectPool
{
private:
	std::vector<IUpdatable*> updatables;
	std::vector<GameObject*> gameObjects;
	std::vector<Renderer*> renderers;
	std::vector<BaseLight*> lights;

public:
	ObjectPool();	

	void AddGameObject(GameObject* obj);
	void AddUpdatable(IUpdatable* upd);
	void AddRenderer(Renderer* rdr);
	void AddLight(BaseLight* objLight);

	void UpdateObjects();
	void RenderObjects(Camera* cam);

	~ObjectPool();
};

