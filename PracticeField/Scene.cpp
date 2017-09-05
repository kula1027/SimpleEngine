#include "Scene.h"

#include "GameObject.h"
#include "Lights.h"
#include "FileLoader.h"
#include "Scene.h"
#include "ArrangedMesh.h"
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

const unsigned int SHADOW_WIDTH = 1024, SHADOW_HEIGHT = 1024;
unsigned int depthMapFBO;
unsigned int depthMap;
void Scene::Load() {	
	current = this;

	GameObject* go;

	int objCount = 5;

	MeshModel* mSph = FileLoader::LoadMeshModel("sphere.obj");
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
				go->transform.position = glm::vec3(
					(loop - (float)objCount / 2) * 5 + 2,
					(loop2) * 5 + 5,
					(-loop3) * 5
				);
			}
		}
	}

	/*
	for (int loop = 0; loop < objCount; loop++) {
		for (int loop2 = 0; loop2 < objCount; loop2++) {
			for (int loop3 = 0; loop3 < objCount; loop3++) {
				GameObject* go = new GameObject();
				go->SetModel(FileLoader::LoadMeshModel("sphere.obj"));
				go->SetRenderer(new Renderer());
				go->SetShader(FileLoader::LoadShader());
				go->transform.position = glm::vec3(
					(loop - objCount / 2) * 5 + 2,
					(loop2) * 5 + 5,
					(-loop3) * 5
				);
			}
		}
	}*/
	
	

	/*mSph = FileLoader::LoadMeshModel("cube.obj");
	for (int i = 0; i < mSph->meshes->size(); i++) {
		mSph->meshes->at(i) = new ArrangedMesh(*mSph->meshes->at(i));
	}
	go = new GameObject();
	go->SetModel(mSph);
	go->SetRenderer(new PartRenderer());
	go->SetShader(FileLoader::LoadShader());
	go->transform.position = glm::vec3(0, 8, 0);*/

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
	
	go = new GameObject();
	go->SetModel(FileLoader::LoadMeshModel("plane.obj"));
	go->SetRenderer(new Renderer());
	go->SetShader(FileLoader::LoadShader());
	go->transform.scale = glm::vec3(20, 1, 20);

	BaseLight* pointLight = new PointLight();
	BaseLight* directionalLight = new DirectionalLight();
	objectPool.AddLight(directionalLight);
	objectPool.AddLight(pointLight);

	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	//glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}


void Scene::UpdateObjects(){
	camera.Update();

	objectPool.UpdateObjects();
}


void Scene::RenderObjectsSinglePass(){
	glViewport(0, 0, SHADOW_WIDTH, SHADOW_HEIGHT);
	glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
	glClear(GL_DEPTH_BUFFER_BIT);
//	ConfigureShaderAndMatrices();
	//RenderScene();
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	
	glClearColor(0.2f, 0.2f, 0.2f, 1);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	objectPool.RenderObjects(&camera);
}

void Scene::RenderObjects(){
	camera.EnableOffSreenDraw();
	
	objectPool.RenderObjects(&camera);

	camera.PostDraw();
}

void ConfigureShaderAndMatrices() {
	
}