#include "Scene.h"

#include "GameObject.h"
#include "Lights.h"
#include "FileManager.h"
#include "Scene.h"
#include "ArrangedMesh.h"
#include "MeshModel.h"
#include "Renderer.h"
#include "PartRenderer.h"
#include "SphereRenderer.h"
#include "InstancedRenderer.h"
#include "MoveCamera.h"
#include "TimeChecker.h"
#include "FakeCam.h"
#include "ImaginaryFigures.h"

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
	std::cout << "Load Scene...\n";
	current = this;

	camera = new Camera();
	camera->AddComponent<MoveCamera>();
	
	NotWonderfulWorld();
	//WonderfulWorld();
	
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

void Scene::WonderfulWorld() {
	GameObject* go;

	//floor
	Texture* t = FileManager::LoadTexture("burnt_sand_brown.png", TextureType_Diffuse);
	MeshModel* mPlane = FileManager::LoadMeshModel_Pool("plane.obj");
	mPlane->meshes->at(0)->textures.push_back(t);
	Mesh* thatMesh = mPlane->meshes->at(0);
	for (int loop = 0; loop < thatMesh->vertices.size(); loop++) {
		thatMesh->vertices[loop].texCoords = 
			glm::vec2(
				thatMesh->vertices[loop].position.x, 
				thatMesh->vertices[loop].position.z
			);
	}
	go = new GameObject("floor");
	go->SetRenderer(new Renderer());	
	go->GetRenderer()->SetMeshModel(mPlane);
	go->GetRenderer()->SetDefaultShader();	
	go->transform->scale = glm::vec3(40, 1, 40);

	//Grass
	Texture* tGrass = FileManager::LoadTexture("grass.png", TextureType_DiffuseTransparent);
	MeshModel* mQuad = FileManager::LoadMeshModel_Pool("quad.obj");
	mQuad->meshes->at(0)->textures.push_back(tGrass);
	thatMesh = mQuad->meshes->at(0);
	for (int loop = 0; loop < thatMesh->vertices.size(); loop++) {
		thatMesh->vertices[loop].texCoords =
			glm::vec2(
				(thatMesh->vertices[loop].position.x + 1) / 2,
				(thatMesh->vertices[loop].position.z + 1) / 2
			);
	}


	GameObject* goGrass = new GameObject("grass parent");
	InstancedRenderer* rdrGrass = new InstancedRenderer();
	goGrass->SetRenderer(rdrGrass);
	
	rdrGrass->castShadow = false;
	rdrGrass->cullingEnabled = false;
	rdrGrass->SetMeshModel(mQuad);
	rdrGrass->SetShader(FileManager::LoadShader("trans_inst_nocull.vert", "trans_inst_nocull.frag"));
	
	int grassCount = 100;
	for (int loop = 0; loop < grassCount; loop++) {
		for (int loop2 = 0; loop2 < grassCount; loop2++) {
			go = new GameObject("grass");
			go->transform->SetParent(goGrass->transform);
			go->transform->scale = glm::vec3(rand() % 3 + 1, 1, rand() % 3 + 1);
			go->transform->position = glm::vec3(-rand() % 400, go->transform->scale.z, -rand() % 400);
			go->transform->SetEulerAngles(glm::vec3(90, rand() % 180, 0));
		}
	}
	rdrGrass->InitInstanced();

	//sphere w geo	
	go = new GameObject("sphere geo");
	go->SetRenderer(new Renderer());
	go->GetRenderer()->SetMeshModel(FileManager::LoadMeshModel_Pool("sphere.obj"));
	go->GetRenderer()->SetShader(FileManager::LoadShader("default.vert", "deform.geo", "default_geo.frag"));
	go->transform->position = glm::vec3(0, 5, 2);

	//sphere	
	go = new GameObject("sphere");
	go->SetRenderer(new Renderer());
	go->GetRenderer()->SetMeshModel(FileManager::LoadMeshModel_Pool("sphere.obj"));
	go->GetRenderer()->SetDefaultShader();
	go->transform->position = glm::vec3(-5, 5, 2);

	//venus
	go = new GameObject("venus");
	go->SetRenderer(new Renderer());
	go->GetRenderer()->SetMeshModel(FileManager::LoadMeshModel_Pool("venusm_wNormal.obj"));
	go->GetRenderer()->SetDefaultShader();
	go->GetRenderer()->outline.draw = true;	
	go->transform->position = glm::vec3(-100, 0, -100);	

	//nanosuit
	go = new GameObject("nano");
	go->SetRenderer(new Renderer());
	go->GetRenderer()->SetMeshModel(FileManager::LoadMeshModel_Pool("nanosuit/nanosuit.obj"));
	go->GetRenderer()->SetDefaultShader();
	go->transform->position = glm::vec3(10, 0, 0);

	//window
	Texture* tWindow = FileManager::LoadTexture("window.png", TextureType_DiffuseTransparent);
	MeshModel* mQuad2 = FileManager::LoadMeshModel("quad.obj");
	mQuad2->meshes->at(0)->textures.push_back(tWindow);
	thatMesh = mQuad2->meshes->at(0);
	for (int loop = 0; loop < thatMesh->vertices.size(); loop++) {
		thatMesh->vertices[loop].texCoords =
			glm::vec2(
			(thatMesh->vertices[loop].position.x + 1) / 2,
				(thatMesh->vertices[loop].position.z + 1) / 2
			);
	}

	go = new GameObject("window");
	go->SetRenderer(new Renderer());
	go->GetRenderer()->castShadow = false;
	go->GetRenderer()->cullingEnabled = false;
	go->GetRenderer()->SetMeshModel(mQuad2);
	go->GetRenderer()->SetShader(FileManager::LoadShader("transparent.vert", "transparent.frag"));
	go->transform->SetEulerAngles(glm::vec3(90, 0, 0));
	go->transform->position = glm::vec3(10, 10, -5);
	go->transform->scale = glm::vec3(2, 1, 2);
}


void Scene::NotWonderfulWorld() {
	GameObject* goTimer = new GameObject("timer");
	goTimer->AddComponent<TimeChecker>();

	GameObject* fakeCam = new GameObject("fakeCam");
	fakeCam->AddComponent<FakeCam>();
	fakeCam->SetRenderer(new Renderer());
	fakeCam->GetRenderer()->SetMeshModel(FileManager::LoadMeshModel("sphere.obj"));
	fakeCam->GetRenderer()->SetDefaultShader();
	fakeCam->transform->position = glm::vec3(0, 10, 10);
	fakeCam->transform->scale = glm::vec3(0.2f);
	fakeCam->GetRenderer()->castShadow = false;	

	char* m[] = {
		"venusm_wNormal.obj",
		"armadillo.obj",
		"bunny.obj",
		"hand.obj",
		"sphere.obj"
	};
	int c = 10;
	for (int loop = 0; loop < 5; loop++) {
		SphereRenderer* srBase = new SphereRenderer();
		srBase->SetMeshModel(FileManager::LoadMeshModel_Pool(m[loop]));
		srBase->renderMaterial->targetCamTr = fakeCam->transform;
		for (int loop2 = 0; loop2 < c; loop2++) {
			GameObject* go = new GameObject(m[loop]);
			SphereRenderer* sr = new SphereRenderer();
			go->SetRenderer(sr);
			go->GetRenderer()->SetMeshModel(FileManager::LoadMeshModel_Pool(m[loop]));
			sr->renderMaterial = srBase->renderMaterial;
			go->GetRenderer()->SetDefaultShader();
			go->GetRenderer()->castShadow = false;			
			go->transform->position = glm::vec3(loop * 10 - 20, 10, -loop2 * 10);
		}
	}

	/*for (int loop = 0; loop < 5; loop++) {
		for (int loop2 = 0; loop2 < c; loop2++) {
			GameObject* go = new GameObject(m[loop]);			
			go->SetRenderer(new Renderer());
			go->GetRenderer()->SetMeshModel(FileManager::LoadMeshModel_Pool(m[loop]));
			go->GetRenderer()->SetDefaultShader();
			go->GetRenderer()->castShadow = false;			
			go->transform->position = glm::vec3(loop * 10 - 20, 10, -loop2 * 10);
		}
	}*/
	//

	////armadillo
	//GameObject* go = new GameObject("armadillo");
	//SphereRenderer* srdr = new SphereRenderer();
	//srdr->castShadow = false;
	//srdr->SetMeshModel(FileManager::LoadMeshModel_Pool("armadillo.obj"));
	//srdr->renderMaterial->targetCamTr = fakeCam->transform;
	//go->SetRenderer(srdr);	
	//go->GetRenderer()->SetDefaultShader();
	//go->transform->position = glm::vec3(10, 10, 0);

	////bunny
	//go = new GameObject("bunny");
	//srdr = new SphereRenderer();
	//srdr->castShadow = false;
	//srdr->SetMeshModel(FileManager::LoadMeshModel_Pool("bunny.obj"));
	//srdr->renderMaterial->targetCamTr = fakeCam->transform;
	//go->SetRenderer(srdr);
	//go->GetRenderer()->SetDefaultShader();
	//go->transform->position = glm::vec3(0, 10, 0);

	////hand
	//go = new GameObject("hand");
	//srdr = new SphereRenderer();
	//srdr->castShadow = false;
	//srdr->SetMeshModel(FileManager::LoadMeshModel_Pool("hand.obj"));
	//srdr->renderMaterial->targetCamTr = fakeCam->transform;
	//go->SetRenderer(srdr);
	//go->GetRenderer()->SetDefaultShader();
	//go->transform->position = glm::vec3(0, 10, 10);

	////sphere
	//go = new GameObject("sphere");
	//srdr = new SphereRenderer();
	//srdr->castShadow = false;
	//srdr->SetMeshModel(FileManager::LoadMeshModel_Pool("sphere.obj"));
	//srdr->renderMaterial->targetCamTr = fakeCam->transform;
	//go->SetRenderer(srdr);
	//go->GetRenderer()->SetDefaultShader();
	//go->transform->position = glm::vec3(-20, 10, 0);
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
	skybox.Render(camera);

	//Render on screen
	glCullFace(GL_BACK);
	camera->PostDraw();
}
