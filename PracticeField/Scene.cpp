#include "Scene.h"

#include "GameObject.h"
#include "Lights.h"
#include "FileLoader.h"
#include "Scene.h"
#include "MeshModel.h"
#include "Renderer.h"
#include "PartRenderer.h"

#include <string>

Scene* Scene::current;

Scene::Scene(){

}

Scene::~Scene(){

}

Scene* Scene::GetCurrent(){
	return current;
}

void Scene::Load() {
	current = this;

	int objCount = 6;
	for (int loop = 0; loop < objCount; loop++) {
		for (int loop2 = 0; loop2 < objCount; loop2++) {
			for (int loop3 = 0; loop3 < objCount; loop3++) {
				GameObject* go = new GameObject();
				go->SetModel(FileLoader::LoadMeshModel("sphere.obj"));
				go->SetRenderer(new PartRenderer());
				go->SetShader(FileLoader::LoadShader());
				go->transform.position = glm::vec3(
					(loop - objCount / 2) * 4,
					(loop2 - objCount / 2) * 4,
					(-loop3) * 4
				);
			}
		}
	}

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
