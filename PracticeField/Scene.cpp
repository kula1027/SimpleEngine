#include "Scene.h"

#include "GameObject.h"
#include "Lights.h"
#include "FileManager.h"
#include "Scene.h"
#include "ArrangedMesh.h"
#include "MeshModel.h"
#include "Renderer.h"
#include "PartRenderer.h"
#include "InstancedRenderer.h"
#include "IUpdatable.h"

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
	glCullFace(GL_FRONT);
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
	
	//BaseLight* pointLight = new PointLight();
	BaseLight* directionalLight = new DirectionalLight();
	AddLight(directionalLight);
	//AddLight(pointLight);	
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

	//floor
	Texture* t = FileManager::LoadTexture("burnt_sand_brown.png", TextureType_Diffuse);
	MeshModel* mPlane = FileManager::LoadMeshModel("plane.obj");
	mPlane->meshes->at(0)->textures.push_back(t);
	Mesh* thatMesh = mPlane->meshes->at(0);
	for (int loop = 0; loop < thatMesh->vertices.size(); loop++) {
		thatMesh->vertices[loop].TexCoords = 
			glm::vec2(
				thatMesh->vertices[loop].Position.x, 
				thatMesh->vertices[loop].Position.z
			);
	}
	go = new GameObject();
	go->SetRenderer(new Renderer());	
	go->GetRenderer()->SetMeshModel(mPlane);
	go->GetRenderer()->SetDefaultShader();	
	go->transform->scale = glm::vec3(40, 1, 40);

	//Grass
	Texture* tGrass = FileManager::LoadTexture("grass.png", TextureType_DiffuseTransparent);
	MeshModel* mQuad = FileManager::LoadMeshModel("quad.obj");
	mQuad->meshes->at(0)->textures.push_back(tGrass);
	thatMesh = mQuad->meshes->at(0);
	for (int loop = 0; loop < thatMesh->vertices.size(); loop++) {
		thatMesh->vertices[loop].TexCoords =
			glm::vec2(
				(thatMesh->vertices[loop].Position.x + 1) / 2,
				(thatMesh->vertices[loop].Position.z + 1) / 2
			);
	}


	GameObject* goGrass = new GameObject();
	InstancedRenderer* rdrGrass = new InstancedRenderer();
	goGrass->SetRenderer(rdrGrass);
	
	rdrGrass->castShadow = false;
	rdrGrass->cullingEnabled = false;
	rdrGrass->SetMeshModel(mQuad);
	rdrGrass->SetShader(FileManager::LoadShader("trans_inst_nocull.vert", "trans_inst_nocull.frag"));
	
	int grassCount = 100;
	for (int loop = 0; loop < grassCount; loop++) {
		for (int loop2 = 0; loop2 < grassCount; loop2++) {
			go = new GameObject();
			go->transform->SetParent(goGrass->transform);
			go->transform->scale = glm::vec3(rand() % 3 + 1, 1, rand() % 3 + 1);
			go->transform->position = glm::vec3(-rand() % 400, go->transform->scale.z, -rand() % 400);
			go->transform->rotation = glm::vec3(90, rand() % 180, 0);
		}
	}
	rdrGrass->InitInstanced();

	//sphere	
	go = new GameObject();
	go->SetRenderer(new Renderer());
	go->GetRenderer()->SetMeshModel(FileManager::LoadMeshModel("sphere.obj"));
	go->GetRenderer()->SetShader(FileManager::LoadShader("default.vert", "deform.geo", "default_geo.frag"));
	go->transform->position = glm::vec3(0, 5, 2);

	//sphere	
	go = new GameObject();
	go->SetRenderer(new Renderer());
	go->GetRenderer()->SetMeshModel(FileManager::LoadMeshModel("sphere.obj"));
	go->GetRenderer()->SetDefaultShader();
	go->transform->position = glm::vec3(-5, 5, 2);

	//venus
	go = new GameObject();
	go->SetRenderer(new Renderer());
	go->GetRenderer()->SetMeshModel(FileManager::LoadMeshModel("venusm_wNormal.obj"));
	go->GetRenderer()->SetDefaultShader();
	go->GetRenderer()->outline.draw = true;	
	go->transform->position = glm::vec3(-100, 0, -100);

	//nanosuit
	go = new GameObject();
	go->SetRenderer(new Renderer());
	go->GetRenderer()->SetMeshModel(FileManager::LoadMeshModel("nanosuit/nanosuit.obj"));
	go->GetRenderer()->SetDefaultShader();
	go->transform->position = glm::vec3(10, 0, 0);

	//window
	Texture* tWindow = FileManager::LoadTexture("window.png", TextureType_DiffuseTransparent);
	MeshModel* mQuad2 = FileManager::LoadMeshModelNoPool("quad.obj");
	mQuad2->meshes->at(0)->textures.push_back(tWindow);
	thatMesh = mQuad2->meshes->at(0);
	for (int loop = 0; loop < thatMesh->vertices.size(); loop++) {
		thatMesh->vertices[loop].TexCoords =
			glm::vec2(
			(thatMesh->vertices[loop].Position.x + 1) / 2,
				(thatMesh->vertices[loop].Position.z + 1) / 2
			);
	}

	go = new GameObject();
	go->SetRenderer(new Renderer());
	go->GetRenderer()->castShadow = false;
	go->GetRenderer()->cullingEnabled = false;
	go->GetRenderer()->SetMeshModel(mQuad2);
	go->GetRenderer()->SetShader(FileManager::LoadShader("transparent.vert", "transparent.frag"));
	go->transform->rotation = glm::vec3(90, 0, 0);
	go->transform->position = glm::vec3(10, 10, -5);
	go->transform->scale = glm::vec3(2, 1, 2);
}


void Scene::NotWonderfulWorld() {

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

	glCullFace(GL_BACK);
	camera.PostDraw();
}
