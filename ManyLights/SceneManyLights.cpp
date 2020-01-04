
#include "SceneManyLights.h"

#include <Scene/SceneIncludes.h>
#include <Render/RenderMaterial/RenderMaterial.h>

SceneManyLights::SceneManyLights() : Scene(){
}

SceneManyLights::~SceneManyLights() {
}

void SceneManyLights::Load() {
	Scene::Load();

	
	mainCamera->GetTransform()->SetPosition(glm::vec3(0, 10, 0));

	EngineObject* eoLight;

	

	//floor	
	MeshModel* mPlane = FilePooler::LoadMeshModel("plane.obj");
	mPlane->meshes->at(0)->renderMaterial->texDiffuse = FilePooler::LoadTexture("asd.png");
	for (int loop = 0; loop < mPlane->meshes->at(0)->vertices.size(); loop++) {
		mPlane->meshes->at(0)->vertices[loop].texCoords =
			glm::vec2(
				mPlane->meshes->at(0)->vertices[loop].position.x,
				mPlane->meshes->at(0)->vertices[loop].position.z
			);
	}
	mPlane->meshes->at(0)->UpdateBuffer();
	EngineObject* eo = new EngineObject("floor");
	MeshRenderer* dRdr = new Renderer_Deferred();	
	//MeshRenderer* dRdr = new Renderer_Forward();
	dRdr->SetMeshModel(mPlane);
	eo->GetTransform()->SetScale(glm::vec3(40, 1, 40));
	eo->AttachComponent(dRdr);

	//sphere	
	for (int loop = 0; loop < 400; loop++) {
		eo = new EngineObject("sphere");
		dRdr = new Renderer_Deferred();
		//dRdr = new Renderer_Forward();
		dRdr->SetMeshModel(FilePooler::LoadMeshModel("sphere.obj"));
		eo->AttachComponent(dRdr);
		eo->GetTransform()->SetPosition(glm::vec3(rand() % 800 - 400, 6, rand() % 800 - 400));
		eo->GetTransform()->SetScale(glm::vec3(5, 5, 5));
	}

	InstantiateLights();
}

void SceneManyLights::InstantiateLights() {
	PointLight* pointLight;
	EngineObject* eoLight;

	int lightCount = 500;

	for (int loop = 0; loop < lightCount; loop++) {
		int rand_range = rand() % 160 + 40;
		float rand_r = (rand() % 128 + 128) * 0.004f;
		float rand_g = (rand() % 128 + 128) * 0.004f;
		float rand_b = (rand() % 128 + 128) * 0.004f;
		float rand_x = (rand() % 800);
		float rand_z = (rand() % 800);

		pointLight = new PointLight();
		pointLight->SetRange(rand_range);
		pointLight->SetColor(vec3(rand_r, rand_g, rand_b));
		pointLight->castShadow = false;

		eoLight = new EngineObject("pLight");
		eoLight->AttachComponent(pointLight);
		eoLight->GetTransform()->SetPosition(vec3(rand_x - 400, 2, rand_z - 400));
	}
}