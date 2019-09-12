#include "Scene.h"

#include "SceneIncludes.h"

#include <string>

Scene::Scene(){

	//TODO RenderPath에 종속되야 할듯함
	//glEnable(GL_DEPTH_TEST);

	//glEnable(GL_BLEND);
	//glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	
	//glEnable(GL_CULL_FACE);
	//glDisable(GL_CULL_FACE);
	//glCullFace(GL_BACK);

	renderData = new SceneRenderData();
}

Scene::~Scene(){
	free(renderData);
}

Scene* Scene::GetCurrent(){
	return SimpleEngine::GetCurrentScene();
}

void Scene::RegisterObject(EngineObject * obj_) {
	SimpleEngine::GetCurrentScene()->AddEngineObject(obj_);
}

void Scene::RegisterRenderer(MeshRenderer *rdr_) {
	SimpleEngine::GetCurrentScene()->AddRenderer(rdr_);
}

void Scene::RegisterLight(BaseLight* light_) {
	SimpleEngine::GetCurrentScene()->AddLight(light_);
}

void Scene::RegisterScript(BaseScript *script_) {
	SimpleEngine::GetCurrentScene()->AddScript(script_);
}

void Scene::Unload() {	
	DebugLog("Unload Scene...");
}

void Scene::Load() {	
	DebugLog("Load Scene...");	

	mainCamera = new Camera();
	mainCamera->AttachComponent(new MoveCamera());
}

EngineObject* Scene::FindEngineObjectByName(string name_) {
	for (int loop = 0; loop < engineObjects.size(); loop++) {
		if (engineObjects[loop]->name.compare(name_)) {
			return engineObjects[loop];
		}
	}

	return NULL;
}

void Scene::AddEngineObject(EngineObject * obj){
	obj->SetId(freeObjectId++);
	engineObjects.push_back(obj);
}

void Scene::AddScript(BaseScript * script_){
	scripts.push_back(script_);
	script_->OnStart();
}

void Scene::AddRenderer(MeshRenderer * rdr_){
	renderData->AddRenderer(rdr_);
}

void Scene::AddLight(BaseLight * light_){
	renderData->AddLight(light_);
}

Camera * Scene::GetMainCamera(){
	return mainCamera;
}

void Scene::UpdateScripts(){
	for (int loop = 0; loop < scripts.size(); loop++) {
		scripts[loop]->OnUpdate();
	}
}

void Scene::RenderScene(){
	mainCamera->Render(renderData);
	
	PerformanceCheck::OnEndFrame();
}
