#include "Scene.h"

#include "GameObject.h"
#include "Lights.h"
#include "FileManager.h"
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
	
	//NotWonderfulWorld();
	WonderfulWorld();
	
	BaseLight* pointLight = new PointLight();
	BaseLight* directionalLight = new DirectionalLight();
	objectPool.AddLight(directionalLight);
	objectPool.AddLight(pointLight);	
}


void Scene::WonderfulWorld() {
	GameObject* go;

	Texture* t = FileManager::LoadTexture("burnt_sand_brown.png", TextureType_Diffuse);
	MeshModel* mPlane = FileManager::LoadMeshModel("plane.obj");
	mPlane->meshes->at(0)->textures.push_back(t);
	Mesh* thatMesh = mPlane->meshes->at(0);
	for (int loop = 0; loop < thatMesh->vertices.size(); loop++) {
		thatMesh->vertices[loop].TexCoords = glm::vec2(thatMesh->vertices[loop].Position.x * 10, thatMesh->vertices[loop].Position.z * 10);
	}
	thatMesh->ResetupMesh();

	go = new GameObject();
	go->SetModel(mPlane);
	go->SetRenderer(new Renderer());
	go->SetShader(FileManager::LoadShader());
	go->transform->scale = glm::vec3(500, 1, 500);

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
	
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	skybox.Render(&camera);
	objectPool.RenderObjects(&camera);
	

	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	camera.PostDraw();
}

void ConfigureShaderAndMatrices() {
	
}