#pragma once

#include "../Skybox/SkyboxBundle.h"

#include <string>
#include <vector>

class MeshModel;
class BaseShader;
class BaseScript;
class EngineObject;
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

	vector<BaseScript*> scripts;
	vector<EngineObject*> engineObjects;
	unsigned long freeObjectId = 0;

	SceneRenderData* renderData;

public:
	Scene();
	~Scene();

	static Scene* GetCurrent();
	static void RegisterObject(EngineObject*);

	virtual void Unload();
	virtual void Load();

	EngineObject * FindEngineObjectByName(string name_);

	void AddEngineObject(EngineObject* obj);
	void AddScript(BaseScript* upd);
	void AddRenderer(BaseRenderer* rdr);
	void AddLight(BaseLight* objLight);

	Camera* GetMainCamera();
		
	void UpdateObjects();
	void RenderScene();

	void SetLineRenderingMode(bool value_);
	void SetFaceCullingMode(bool value_);
};