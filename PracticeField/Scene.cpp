#include "Scene.h"


Scene::Scene(GameWindow* gWindow_){
	currentWindow = gWindow_;

	objectPool = new ObjectPool();
	camera = new Camera();
}


Scene::~Scene(){
}

void Scene::Load() {
	objectPool->AddObejct(new GameObject());
}


void Scene::UpdateObjects(){
	camera->Update(currentWindow);

	objectPool->UpdateObjects();
}

void Scene::RenderObjects(){
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	objectPool->RenderObjects(camera);
}
