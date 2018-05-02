#pragma once

#include "Skybox.h"

#include <string>
#include <vector>

class MeshModel;
class Shader;
class IUpdatable;
class GameObject;
class BaseLight;
class Renderer;
class Camera;

using namespace std;

class Scene
{
private:
	static Scene* current;
	SkyBox* skybox;
	Camera* camera;
	
	vector<IUpdatable*> updatables;
	vector<GameObject*> gameObjects;
	unsigned int freeObjectId = 0;

	vector<Renderer*> renderers;
	vector<BaseLight*> lights;

public:
	Scene();
	~Scene();

	static Scene* GetCurrent();	

	void Load();

	GameObject * FindGameObjectByName(string name_);

	void AddGameObject(GameObject* obj);
	void AddUpdatable(IUpdatable* upd);
	void AddRenderer(Renderer* rdr);
	void AddLight(BaseLight* objLight);

	Camera* GetCamera();
		
	void UpdateObjects();
	void RenderObjects();

	void WonderfulWorld();
	void NotWonderfulWorld();
};