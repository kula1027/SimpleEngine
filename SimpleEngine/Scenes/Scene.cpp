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

void Scene::RegisterObject(EngineObject * obj_) {
	current->AddEngineObject(obj_);
}

void Scene::Unload() {
	SP_Debugger::Log("Unload Scene...");
}

void Scene::Load() {
	SP_Debugger::Log("Load Scene...");
	current = this;

	mainCamera = new Camera();
	mainCamera->AddComponent<MoveCamera>();	
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
	//cout << "Add GameObject " << obj->name << endl;
	obj->SetId(freeObjectId++);
	engineObjects.push_back(obj);
}

void Scene::AddScript(BaseScript * scpt){
	scripts.push_back(scpt);
	scpt->OnStart();
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
	for (int loop = 0; loop < scripts.size(); loop++) {
		scripts[loop]->OnUpdate();
	}
}

void Scene::RenderScene(){
	mainCamera->Render(renderData);
	
}

void Scene::SetLineRenderingMode(bool value_) {
	if (value_) {
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	} else {
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	}
}

void Scene::SetFaceCullingMode(bool value_) {
	if (value_) {
		glEnable(GL_CULL_FACE);
	} else {
		glDisable(GL_CULL_FACE);
	}
}
