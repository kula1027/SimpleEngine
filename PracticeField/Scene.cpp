#include "Scene.h"

#include "GameObject.h"
#include "Lights.h"
#include "FileLoader.h"
#include "Scene.h"
#include "MeshModel.h"

#include <string>

Scene::Scene(){

}

Scene::~Scene(){
	
}

void Scene::Load() {
	GameObject* go = new GameObject();
	MeshModel* meshModel = FileLoader::LoadMeshModel("sphere.obj");
	go->SetModel(meshModel);
	go->SetShader(FileLoader::LoadShader("defaultVertex.vertex", "defaultFragment.fragment"));	

	objectPool.AddGameObject(go);

	/*
	go = new GameObject();
	meshModel = FileLoader::LoadMeshModel("untitled.obj");
	go->SetModel(meshModel);
	go->SetShader(FileLoader::LoadShader("defaultVertex.vertex", "defaultFragment.fragment"));
	go->transform.position = glm::vec3(0, 0, 0);
	go->transform.scale = glm::vec3(100, 1, 100);*/
	//objectPool.AddGameObject(go);

	BaseLight* pointLight = new PointLight();
	BaseLight* directionalLight = new DirectionalLight();
	objectPool.AddLight(directionalLight);
	objectPool.AddLight(pointLight);
}


void Scene::UpdateObjects(){
	camera.Update();

	objectPool.UpdateObjects();
}

void Scene::RenderObjects(){
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	objectPool.RenderObjects(&camera);
}
