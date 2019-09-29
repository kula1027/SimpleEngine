#include "TestScene.h"

#include <Scenes/SceneIncludes.h>

#include <string>
#include <Render/RenderMaterial/RenderMaterial.h>

TestScene::TestScene() : Scene()
{
}


TestScene::~TestScene()
{
}

void TestScene::Load()
{
	Scene::Load();

	EngineObject* eoLight = new EngineObject("light");
	DirectionalLight* directionalLight = new DirectionalLight();
	eoLight->AttachComponent(directionalLight);

	mainCamera->SetSkybox(new SkyBox());	
	mainCamera->transform->position = glm::vec3(0, 10, 0);

	EngineObject* eo;

	//floor
	Texture* t = FilePooler::LoadTexture("../Materials/burnt_sand_brown.png");
	MeshModel* mPlane = FilePooler::LoadMeshModel("plane.obj");
	mPlane->meshes->at(0)->renderMaterial->texDiffuse = t;
	Mesh* thatMesh = mPlane->meshes->at(0);
	for (int loop = 0; loop < thatMesh->vertices.size(); loop++) {
		thatMesh->vertices[loop].texCoords =
			glm::vec2(
				thatMesh->vertices[loop].position.x,
				thatMesh->vertices[loop].position.z
			);
	}
	eo = new EngineObject("floor");
	MeshRenderer* dRdr = new MeshRenderer();
	eo->AttachComponent(dRdr);	
	dRdr->SetMeshModel(mPlane);
	eo->transform->scale = glm::vec3(40, 1, 40);


	//nanosuit
	eo = new EngineObject("nano");
	dRdr = new MeshRenderer();
	eo->AttachComponent(dRdr);
	dRdr->SetMeshModel(FilePooler::LoadMeshModel("nanosuit/nanosuit.obj"));	
	eo->transform->position = glm::vec3(0, 0, -30);


	
	//Grass
	//Texture* tGrass = FilePooler::LoadTexture("../Materials/grass.png", TextureType_DiffuseTransparent);
	//MeshModel* mQuad = FilePooler::LoadMeshModel("quad.obj");
	//mQuad->meshes->at(0)->textures.push_back(tGrass);
	//thatMesh = mQuad->meshes->at(0);
	//for (int loop = 0; loop < thatMesh->vertices.size(); loop++) {
	//	thatMesh->vertices[loop].texCoords =
	//		glm::vec2(
	//		(thatMesh->vertices[loop].position.x + 1) / 2,
	//			(thatMesh->vertices[loop].position.z + 1) / 2
	//		);
	//}


	//EngineObject* goGrass = new EngineObject("grass parent");
	//InstancedRenderer* rdrGrass = new InstancedRenderer();
	//goGrass->AttachComponent(rdrGrass);	

	//rdrGrass->castShadow = false;
	////rdrGrass->cullingEnabled = false;
	//rdrGrass->SetMeshModel(mQuad);
	//rdrGrass->SetShader(FilePooler::LoadShader("trans_inst_nocull.vert", "trans_inst_nocull.frag"));

	//int grassCount = 100;
	//for (int loop = 0; loop < grassCount; loop++) {
	//	for (int loop2 = 0; loop2 < grassCount; loop2++) {
	//		eo = new EngineObject("grass");
	//		eo->transform->SetParent(goGrass->transform);
	//		eo->transform->scale = glm::vec3(rand() % 3 + 1, 1, rand() % 3 + 1);
	//		eo->transform->position = glm::vec3(-rand() % 400, eo->transform->scale.z, -rand() % 400);
	//		eo->transform->SetEulerAngles(glm::vec3(90, rand() % 180, 0));
	//	}
	//}
	//rdrGrass->InitInstanced();

	////sphere w geo	
	//go = new GameObject("sphere geo");
	//go->SetRenderer(new MeshRenderer());
	//go->GetRenderer()->SetMeshModel(FilePooler::LoadMeshModel("sphere.obj"));
	//go->GetRenderer()->SetShader(FilePooler::LoadShader("default.vert", "deform.geo", "default_geo.frag"));
	//go->transform->position = glm::vec3(0, 5, 2);

	////sphere	
	//go = new GameObject("sphere");
	//go->SetRenderer(new MeshRenderer());
	//go->GetRenderer()->SetMeshModel(FilePooler::LoadMeshModel("sphere.obj"));
	//go->GetRenderer()->SetShader();
	//go->transform->position = glm::vec3(-5, 5, 2);

	////venus
	//go = new GameObject("venus");
	//go->SetRenderer(new OutlineRenderer());
	//go->GetRenderer()->SetMeshModel(FilePooler::LoadMeshModel("venusm_wNormal.obj"));
	//go->GetRenderer()->SetShader();	
	//go->transform->position = glm::vec3(-100, 0, -100);

	////window
	//Texture* tWindow = FilePooler::LoadTexture("window.png", TextureType_DiffuseTransparent);
	//MeshModel* mQuad2 = FilePooler::LoadMeshModel("quad.obj");
	//mQuad2->meshes->at(0)->textures.push_back(tWindow);
	//thatMesh = mQuad2->meshes->at(0);
	//for (int loop = 0; loop < thatMesh->vertices.size(); loop++) {
	//	thatMesh->vertices[loop].texCoords =
	//		glm::vec2(
	//		(thatMesh->vertices[loop].position.x + 1) / 2,
	//			(thatMesh->vertices[loop].position.z + 1) / 2
	//		);
	//}

	//go = new GameObject("window");
	//go->SetRenderer(new MeshRenderer());
	//go->GetRenderer()->castShadow = false;
	//go->GetRenderer()->cullingEnabled = false;
	//go->GetRenderer()->SetMeshModel(mQuad2);
	//go->GetRenderer()->SetShader(FilePooler::LoadShader("transparent.vert", "transparent.frag"));
	//go->transform->SetEulerAngles(glm::vec3(90, 0, 0));
	//go->transform->position = glm::vec3(10, 10, -5);
	//go->transform->scale = glm::vec3(2, 1, 2);

	
}
