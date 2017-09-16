#pragma once

#include "Camera.h"
#include "Skybox.h"

#include <string>
#include <vector>

class MeshModel;
class Shader;
class IUpdatable;
class GameObject;
class BaseLight;
class Renderer;

using namespace std;

class Scene
{
private:
	static Scene* current;
	Skybox skybox;
	Camera camera;
	
	vector<IUpdatable*> updatables;
	vector<GameObject*> gameObjects;
	vector<Renderer*> renderers;
	vector<BaseLight*> lights;

public:
	Scene();
	~Scene();

	static Scene* GetCurrent();	

	void Load();

	void AddGameObject(GameObject* obj);
	void AddUpdatable(IUpdatable* upd);
	void AddRenderer(Renderer* rdr);
	void AddLight(BaseLight* objLight);
		
	void UpdateObjects();
	void RenderObjectsSinglePass();	
	void RenderObjects();

	void WonderfulWorld();
	void NotWonderfulWorld();
};