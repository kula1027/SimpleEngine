#pragma once

#include "../Skybox/SkyboxBundle.h"

#include <string>
#include <vector>

class MeshModel;
class BaseShader;
class BaseScript;
class EngineObject;
class BaseLight;
class MeshRenderer;
class Camera;
class SceneRenderData;

using namespace std;

class Scene
{
private:
	void AddEngineObject(EngineObject* obj);
	void AddScript(BaseScript* upd);
	void AddRenderer(MeshRenderer* rdr);
	void AddLight(BaseLight* objLight);

protected:
	Camera* mainCamera;

	vector<BaseScript*> scripts;
	vector<EngineObject*> engineObjects;
	unsigned long freeObjectId = 0;

	SceneRenderData* renderData;

public:
	Scene();
	virtual ~Scene();

	static Scene* GetCurrent();
	static void RegisterObject(EngineObject*);
	static void RegisterRenderer(MeshRenderer*);
	static void RegisterLight(BaseLight*);
	static void RegisterScript(BaseScript*);

	virtual void Unload();
	virtual void Load();

	EngineObject * FindEngineObjectByName(string name_);	

	Camera* GetMainCamera();
		
	void UpdateScripts();
	void RenderScene();
};