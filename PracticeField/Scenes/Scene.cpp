#include "Scene.h"

#include "SceneIncludes.h"

#include <string>

Scene* Scene::current;

Scene::Scene(){
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);
	//glEnable(GL_STENCIL_TEST);
	glStencilFunc(GL_NOTEQUAL, 1, 0xFF);
	glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	
	glEnable(GL_CULL_FACE);
	//glDisable(GL_CULL_FACE);
	glCullFace(GL_FRONT);	

	renderData = new SceneRenderData();
}

Scene::~Scene(){
}

Scene* Scene::GetCurrent(){
	return current;
}

void Scene::Load() {	
	std::cout << "Load Scene...\n";
	current = this;

	mainCamera = new Camera();
	mainCamera->AddComponent<MoveCamera>();	
}

void Scene::Initialize() {
	FilePooler::InitializeShaders();

	for (int loop = 0; loop < gameObjects.size(); loop++) {
		gameObjects[loop]->Initialize();
	}

	for (int loop = 0; loop < updatables.size(); loop++) {
		updatables[loop]->Initialize();
	}

	for (int loop = 0; loop < updatables.size(); loop++) {
		updatables[loop]->OnStart();
	}
}

GameObject* Scene::FindGameObjectByName(string name_) {
	for (int loop = 0; loop < gameObjects.size(); loop++) {
		if (gameObjects[loop]->name.compare(name_)) {
			return gameObjects[loop];
		}
	}

	return NULL;
}

void Scene::AddGameObject(GameObject * obj){
	//cout << "Add GameObject " << obj->name << endl;
	obj->SetId(freeObjectId++);
	gameObjects.push_back(obj);
}

void Scene::AddUpdatable(IUpdatable * upd){
	updatables.push_back(upd);
}

void Scene::AddRenderer(BaseRenderer * rdr){
	renderData->AddRenderer(rdr);
}

void Scene::AddLight(BaseLight * light_){
	renderData->AddLight(light_);
}

Camera * Scene::GetMainCamera(){
	return mainCamera;
}

void Scene::UpdateObjects(){
	for (int loop = 0; loop < updatables.size(); loop++) {
		updatables[loop]->OnUpdate();
	}
}

void Scene::RenderScene(){
	mainCamera->Render(renderData);	
}
