#include "TestScene2.h"

#include "SceneIncludes.h"

#include <string>

TestScene2::TestScene2() : Scene()
{
}


TestScene2::~TestScene2()
{
}

void TestScene2::Load()
{
	Scene::Load();

	BaseLight* directionalLight = new DirectionalLight();
	AddLight(directionalLight);

	skybox = new SkyBox();

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
	go->GetRenderer()->SetShader();
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
	go->GetRenderer()->SetShader();
	go->transform->position = glm::vec3(-5, 5, 2);

	//venus
	go = new GameObject("venus");
	go->SetRenderer(new OutlineRenderer());
	go->GetRenderer()->SetMeshModel(FileManager::LoadMeshModel_Pool("venusm_wNormal.obj"));
	go->GetRenderer()->SetShader();	
	go->transform->position = glm::vec3(-100, 0, -100);

	//nanosuit
	go = new GameObject("nano");
	go->SetRenderer(new Renderer());
	go->GetRenderer()->SetMeshModel(FileManager::LoadMeshModel_Pool("nanosuit/nanosuit.obj"));
	go->GetRenderer()->SetShader();
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
