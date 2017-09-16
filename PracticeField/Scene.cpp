#include "Scene.h"

#include "GameObject.h"
#include "Lights.h"
#include "FileManager.h"
#include "Scene.h"
#include "ArrangedMesh.h"
#include "MeshModel.h"
#include "Renderer.h"
#include "PartRenderer.h"
#include "IUpdatable.h"

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

	GameObject* go;
	
	//NotWonderfulWorld();
	WonderfulWorld();
	
	BaseLight* pointLight = new PointLight();
	BaseLight* directionalLight = new DirectionalLight();
	AddLight(directionalLight);
	AddLight(pointLight);	
}

void Scene::AddGameObject(GameObject * obj){
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


void Scene::WonderfulWorld() {
	GameObject* go;

	Texture* t = FileManager::LoadTexture("burnt_sand_brown.png", TextureType_Diffuse);
	MeshModel* mPlane = FileManager::LoadMeshModel("plane.obj");
	mPlane->meshes->at(0)->textures.push_back(t);
	Mesh* thatMesh = mPlane->meshes->at(0);
	for (int loop = 0; loop < thatMesh->vertices.size(); loop++) {
		thatMesh->vertices[loop].TexCoords = glm::vec2(thatMesh->vertices[loop].Position.x, thatMesh->vertices[loop].Position.z);
	}
	thatMesh->ResetupMesh();

	go = new GameObject();
	go->SetModel(mPlane);
	go->SetRenderer(new Renderer());
	go->SetShader(FileManager::LoadShader());
	go->transform->scale = glm::vec3(1, 1, 1);

	go = new GameObject();
	go->SetModel(FileManager::LoadMeshModel("nanosuit/nanosuit.obj"));
	go->SetRenderer(new Renderer());
	go->SetShader(FileManager::LoadShader());
}


void Scene::NotWonderfulWorld() {
	int objCount = 0;
	GameObject* go;

	/*MeshModel* mSph = FileLoader::LoadMeshModel("sphere.obj");
	for (int i = 0; i < mSph->meshes->size(); i++) {
	mSph->meshes->at(i) = new ArrangedMesh(*mSph->meshes->at(i));
	}

	for (int loop = 0; loop < objCount; loop++) {
	for (int loop2 = 0; loop2 < objCount; loop2++) {
	for (int loop3 = 0; loop3 < objCount; loop3++) {
	GameObject* go = new GameObject();
	go->SetModel(mSph);
	go->SetRenderer(new PartRenderer());
	go->SetShader(FileLoader::LoadShader());
	go->transform->position = glm::vec3(
	(loop - (float)objCount / 2) * 5 + 2,
	(loop2) * 5 + 5,
	(-loop3) * 5
	);
	}
	}
	}*/

	/*mSph = FileLoader::LoadMeshModel("venusm.obj");
	for (int i = 0; i < mSph->meshes->size(); i++) {
	mSph->meshes->at(i) = new ArrangedMesh(*mSph->meshes->at(i));
	}
	go = new GameObject();
	go->SetModel(mSph);
	go->SetRenderer(new PartRenderer());
	go->SetShader(FileLoader::LoadShader());
	go->transform.scale = glm::vec3(0.005f, 0.005f, 0.005f);
	go->transform.position = glm::vec3(-20, 8, 0);*/


	for (int loop = 0; loop < objCount; loop++) {
		for (int loop2 = 0; loop2 < objCount; loop2++) {
			for (int loop3 = 0; loop3 < objCount; loop3++) {
				GameObject* go = new GameObject();
				go->SetModel(FileManager::LoadMeshModel("sphere.obj"));
				go->SetRenderer(new Renderer());
				go->SetShader(FileManager::LoadShader());
				go->transform->position = glm::vec3(
					(loop - objCount / 2) * 5 + 2,
					(loop2) * 5 + 5,
					(-loop3) * 5
				);
			}
		}
	}
}

void Scene::UpdateObjects(){
	camera.Update();

	for (int loop = 0; loop < updatables.size(); loop++) {
		updatables[loop]->Update();
	}
}


void Scene::RenderObjectsSinglePass(){
	glClearColor(0.2f, 0.2f, 0.2f, 1);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	int rSize = renderers.size();
	for (int loop = 0; loop < rSize; loop++) {
		renderers[loop]->Render(&camera, lights);
	}
}

void Scene::RenderObjects(){
	int lightCount = lights.size();
	int rdrCount = renderers.size();

	glEnable(GL_DEPTH_TEST);
	for (int loop = 0; loop < lightCount; loop++) {		
		lights[loop]->EnableShadowMapBuffer();		
		for (int loop2 = 0; loop2 < rdrCount; loop2++) {
			renderers[loop2]->RenderShadowMap(lights[loop]);
		}
	}

	camera.EnableOffSreenBuffer();
	for (int loop = 0; loop < rdrCount; loop++) {
		renderers[loop]->Render(&camera, lights);
	}
	skybox.Render(&camera);

	camera.PostDraw();
}
