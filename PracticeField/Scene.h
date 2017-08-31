#pragma once

#include "ObjectPool.h"
#include "Camera.h"

#include <string>
#include <vector>

class MeshModel;
class Shader;

class Scene
{
private:
	static Scene* current;
	
	Camera camera;
	
public:
	Scene();
	~Scene();

	static Scene* GetCurrent();	
	ObjectPool objectPool;//TODO: to pointer var

	void Load();
		
	void UpdateObjects();
	void RenderObjectsSinglePass();	
	void RenderObjects();
};