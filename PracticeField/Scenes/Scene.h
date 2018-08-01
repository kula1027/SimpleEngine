#pragma once

#include "../Skybox/SkyboxBundle.h"

#include <string>
#include <vector>

class MeshModel;
class BaseShader;
class IUpdatable;
class GameObject;
class BaseLight;
class BaseRenderer;
class Camera;
class SceneRenderData;

using namespace std;

class Scene
{
private:
	static Scene* current;

protected:
	Camera* mainCamera;

	vector<IUpdatable*> updatables;
	vector<GameObject*> gameObjects;
	unsigned long freeObjectId = 0;

	SceneRenderData* renderData;/*
	vector<Renderer*> renderers;
	vector<BaseLight*> lights;	*/

public:
	Scene();
	~Scene();

	static Scene* GetCurrent();	

	virtual void Load();
	void Initialize();

	GameObject * FindGameObjectByName(string name_);

	void AddGameObject(GameObject* obj);
	void AddUpdatable(IUpdatable* upd);
	void AddRenderer(BaseRenderer* rdr);
	void AddLight(BaseLight* objLight);

	Camera* GetMainCamera();
		
	void UpdateObjects();
	void RenderScene();
};