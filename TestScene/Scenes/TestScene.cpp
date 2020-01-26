#include "TestScene.h"

#include <Scene/SceneIncludes.h>

#include <string>
#include <Render/RenderMaterial/RenderMaterial.h>

TestScene::TestScene() : Scene() {
}


TestScene::~TestScene() {
}

void TestScene::Load() {
	Scene::Load();

	mainCamera->SetSkybox(new SkyBox());
	mainCamera->GetTransform()->SetPosition(glm::vec3(0, 10, 20));

	EngineObject* eoLight;

	eoLight = new EngineObject("light");
	DirectionalLight* directionalLight = new DirectionalLight();
	eoLight->AttachComponent(directionalLight);
	eoLight->GetTransform()->SetForward(glm::vec3(0.7, -0.7, 0));

	PointLight* pointLight = new PointLight();
	pointLight->SetRange(10);
	eoLight = new EngineObject("pLight");
	eoLight->AttachComponent(pointLight);
	eoLight->GetTransform()->SetPosition(vec3(-2.5f, 0.5f, 0));

	EngineObject* eo;

	//floor	
	MeshModel* mPlane = FilePooler::LoadMeshModel("plane.obj");
	mPlane->meshes->at(0)->renderMaterial->texDiffuse = FilePooler::LoadTexture("burnt_sand_brown.png");
	for (int loop = 0; loop < mPlane->meshes->at(0)->vertices.size(); loop++) {
		mPlane->meshes->at(0)->vertices[loop].texCoords =
			glm::vec2(
				mPlane->meshes->at(0)->vertices[loop].position.x,
				mPlane->meshes->at(0)->vertices[loop].position.z
			);
	}
	mPlane->meshes->at(0)->UpdateBuffer();
	eo = new EngineObject("floor");
	//MeshRenderer* dRdr = new Renderer_Deferred();
	MeshRenderer* dRdr = new Renderer_Forward();
	dRdr->SetMeshModel(mPlane);
	eo->GetTransform()->SetScale(glm::vec3(40, 1, 40));
	eo->AttachComponent(dRdr);

	//nanosuit
	eo = new EngineObject("nano");
	dRdr = new Renderer_Deferred();
	dRdr->SetMeshModel(FilePooler::LoadMeshModel("nanosuit/nanosuit.obj"));
	eo->AttachComponent(dRdr);
	eo->GetTransform()->SetPosition(glm::vec3(0, 0, 0));

	//sphere	
	eo = new EngineObject("sphere");
	dRdr = new Renderer_Forward();
	dRdr->SetMeshModel(FilePooler::LoadMeshModel("sphere.obj"));
	eo->AttachComponent(dRdr);
	eo->GetTransform()->SetPosition(glm::vec3(-5, 2, 0));

	//sphere	
	eo = new EngineObject("sphere");
	dRdr = new Renderer_Deferred();
	dRdr->SetMeshModel(FilePooler::LoadMeshModel("sphere.obj"));
	eo->AttachComponent(dRdr);
	eo->GetTransform()->SetPosition(glm::vec3(-8, 2, 0));

	eo = new EngineObject("sphere");
	dRdr = new Renderer_Deferred();
	dRdr->SetMeshModel(FilePooler::LoadMeshModel("Sphere/sphere_64_32.obj"));
	eo->AttachComponent(dRdr);
	eo->GetTransform()->SetPosition(glm::vec3(-11, 2, 0));

	EngineObject* eoInst = new EngineObject("sphere Inst");
	Renderer_Instanced* rdrInst = new Renderer_Instanced();
	rdrInst->SetMeshModel(FilePooler::LoadMeshModel("Sphere/sphere_64_32.obj"));
	eoInst->AttachComponent(rdrInst);
	eoInst->GetTransform()->SetPosition(glm::vec3());
	int grassCount = 10;
	for (int loop = 0; loop < grassCount; loop++) {
		for (int loop2 = 0; loop2 < grassCount; loop2++) {
			EngineObject* eoChild = new EngineObject("grass");
			eoChild->GetTransform()->SetParent(eoInst->GetTransform());
			//eoChild->GetTransform()->SetScale(glm::vec3(rand() % 3 + 1, 1, rand() % 3 + 1));
			eoChild->GetTransform()->SetPosition(glm::vec3(-rand() % 40 + 20, 1, -rand() % 40 + 20));
			eoChild->GetTransform()->SetEulerAngles(glm::vec3(90, rand() % 180, 0));
		}
	}
	rdrInst->InitInstanced();

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

	////sphere w geo	
	//go = new GameObject("sphere geo");
	//go->SetRenderer(new MeshRenderer());
	//go->GetRenderer()->SetMeshModel(FilePooler::LoadMeshModel("sphere.obj"));
	//go->GetRenderer()->SetShader(FilePooler::LoadShader("default.vert", "deform.geo", "default_geo.frag"));
	//go->transform->GetPosition() = glm::vec3(0, 5, 2);	

	////venus
	//go = new GameObject("venus");
	//go->SetRenderer(new OutlineRenderer());
	//go->GetRenderer()->SetMeshModel(FilePooler::LoadMeshModel("venusm_wNormal.obj"));
	//go->GetRenderer()->SetShader();	
	//go->transform->GetPosition() = glm::vec3(-100, 0, -100);

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
	//go->transform->GetPosition() = glm::vec3(10, 10, -5);
	//go->transform->scale = glm::vec3(2, 1, 2);


}
