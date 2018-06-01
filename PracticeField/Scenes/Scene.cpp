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
}

Scene::~Scene(){

}

Scene* Scene::GetCurrent(){
	return current;
}

void Scene::Load() {	
	std::cout << "Load Scene...\n";
	current = this;

	camera = new Camera();
	camera->AddComponent<MoveCamera>();	
	
	//BaseLight* pointLight = new PointLight();
	BaseLight* directionalLight = new DirectionalLight();
	AddLight(directionalLight);
	//AddLight(pointLight);
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

void Scene::AddRenderer(Renderer * rdr){
	renderers.push_back(rdr);
}

void Scene::AddLight(BaseLight * objLight){
	lights.push_back(objLight);
}

Camera * Scene::GetCamera(){
	return camera;
}

void Scene::UpdateObjects(){
	for (int loop = 0; loop < updatables.size(); loop++) {
		updatables[loop]->OnUpdate();
	}
}


void Scene::RenderObjects(){
	int lightCount = lights.size();
	int rdrCount = renderers.size();
	
	//Matrice Setup
	camera->ComputeMatrix();
	for (int loop = 0; loop < rdrCount; loop++) {
		renderers[loop]->ComputeMatrix();
	}

	//Render ShadowMap
	for (int loop = 0; loop < lightCount; loop++) {	
		lights[loop]->EnableShadowMapBuffer();		
		for (int loop2 = 0; loop2 < rdrCount; loop2++) {
			renderers[loop2]->RenderShadowMap(lights[loop]);
		}
	}

	//Render Off Screen
	camera->EnableOffSreenBuffer();	
	for (int loop = 0; loop < rdrCount; loop++) {
		renderers[loop]->Render(camera, lights);
	}
	skybox->Render(camera);

	//Render on screen
	glCullFace(GL_BACK);
	camera->PostDraw();
}
