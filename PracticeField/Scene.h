#pragma once

#include "ObjectPool.h"
#include "Camera.h"
#include "GameObject.h"
#include "PointLight.h"

class Scene
{
private:
	ObjectPool objectPool;
	Camera camera;

	vector<MeshModel*> meshModelPool;
	vector<Shader*> shaderPool;

	MeshModel* LoadMesh(string path);
	Shader* LoadShader();

public:
	Scene();
	~Scene();

	void Load();
		
	void UpdateObjects();
	void RenderObjects();	
};