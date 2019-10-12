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


	mainCamera->SetSkybox(new SkyBox());	
	mainCamera->transform->position = glm::vec3(0, 10, 20);	

	EngineObject* eoLight;

	PointLight* pointLight;
	for (int loop = 0; loop < 300; loop++) {

		int rand_range = rand() % 40 + 20;
		float rand_r = (rand() % 128 + 128) * 0.004f;
		float rand_g = (rand() % 128 + 128) * 0.004f;
		float rand_b = (rand() % 128 + 128) * 0.004f;
		float rand_x = (rand() % 401 - 200);
		float rand_z = (rand() % 401 - 200);

		pointLight = new PointLight();
		pointLight->SetRange(rand_range);
		pointLight->SetColor(vec3(rand_r, rand_g, rand_b));

		eoLight = new EngineObject("pLight");
		eoLight->AttachComponent(pointLight);
		eoLight->transform->SetPosition(vec3(rand_x, 1, rand_z));
	}
	

	eoLight = new EngineObject("light");
	DirectionalLight* directionalLight = new DirectionalLight();
	eoLight->AttachComponent(directionalLight);
	eoLight->transform->SetForward(glm::vec3(0.7, -0.7, 0));

	EngineObject* eo;

	//floor
	Texture* t = FilePooler::LoadTexture("../Materials/blackone.png");
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
	dRdr->SetMeshModel(FilePooler::LoadMeshModel("nanosuit/nanosuit.obj"));
	eo->AttachComponent(dRdr);	
	eo->transform->position = glm::vec3(0, 0, 0);

	//sphere	
	eo = new EngineObject("sphere");
	dRdr = new MeshRenderer();
	dRdr->SetMeshModel(FilePooler::LoadMeshModel("sphere.obj"));
	dRdr->SetRenderModeForward(true);
	eo->AttachComponent(dRdr);
	eo->transform->position = glm::vec3(-5, 2, 0);

	//sphere	
	eo = new EngineObject("sphere");
	dRdr = new MeshRenderer();
	dRdr->SetMeshModel(FilePooler::LoadMeshModel("sphere.obj"));
	//dRdr->SetRenderModeForward(true);
	eo->AttachComponent(dRdr);
	eo->transform->position = glm::vec3(-7, 2, 0);

	eo = new EngineObject("sphere");
	dRdr = new MeshRenderer();
	dRdr->SetMeshModel(FilePooler::LoadMeshModel("Sphere/sphere_64_32.obj"));
	//dRdr->SetRenderModeForward(true);
	eo->AttachComponent(dRdr);
	eo->transform->position = glm::vec3(14, 5, 0);

	
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
