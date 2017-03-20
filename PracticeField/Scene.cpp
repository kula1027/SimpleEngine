#include "Scene.h"


Scene::Scene(){

}


Scene::~Scene(){
}

void Scene::Load() {
	GameObject* go = new GameObject();
	go->SetModel(MaterialLoader::LoadMeshModel("nanosuit/nanosuit.blend"));
	go->SetShader(MaterialLoader::LoadShader("shaderVertex.shader", "shaderFragment.fragment"));
	objectPool.AddGameObject(go);

	Light* pointLight = new PointLight();
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
