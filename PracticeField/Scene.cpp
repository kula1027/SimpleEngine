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

	
	MeshModel* meshModel = LoadMesh("Rock/Rock.obj");
	go->SetModel(meshModel);
	go->SetShader(FileLoader::LoadShader("defaultVertex.vertex", "defaultFragment.fragment"));
	go->transform.position = glm::vec3(0, 1, 0);
	go->transform.scale = glm::vec3(0.1f, 0.1f, 0.1f);
	objectPool.AddGameObject(go);

	go = new GameObject();
	meshModel = LoadMesh("sphere.obj");
	go->SetModel(meshModel);
	go->SetShader(FileLoader::LoadShader("defaultVertex.vertex", "defaultFragment.fragment"));	
	go->transform.position = glm::vec3(20, 10, 0);
	objectPool.AddGameObject(go);
	
	go = new GameObject();
	meshModel = LoadMesh("untitled.obj");
	go->SetModel(meshModel);
	go->SetShader(FileLoader::LoadShader("defaultVertex.vertex", "defaultFragment.fragment"));
	go->transform.position = glm::vec3(0, 0, 0);
	go->transform.scale = glm::vec3(100, 1, 100);
	objectPool.AddGameObject(go);

	BaseLight* pointLight = new PointLight();
	BaseLight* directionalLight = new DirectionalLight();
	objectPool.AddLight(directionalLight);
	objectPool.AddLight(pointLight);
}

MeshModel * Scene::LoadMesh(std::string path){	
	MeshModel* foundMesh = NULL;

	for (int loop = 0; loop < meshModelPool.size(); loop++) {
		if (meshModelPool[loop]->GetDirectory().compare(path.c_str()) == 0) {			
			foundMesh = meshModelPool[loop];
			break;
		}
	}

	if (foundMesh != NULL) {		
		return foundMesh;
	}else {
		MeshModel* meshModel = FileLoader::LoadMeshModel(path);
		meshModelPool.push_back(meshModel);

		return meshModel;
	}
}

Shader * Scene::LoadShader(){
	return nullptr;
}


void Scene::UpdateObjects(){
	camera.Update();

	objectPool.UpdateObjects();
}

void Scene::RenderObjects(){
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	objectPool.RenderObjects(&camera);
}
