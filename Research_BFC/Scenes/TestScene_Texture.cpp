#include "TestScene_Texture.h"

#include <Scenes/SceneIncludes.h>

#include <string>

#include "../Renderer/PreCullingRenderer.h"
#include "../Renderer/PreCullingRenderer_Split.h"
#include "../Scripts/FakeCam.h"

TestScene_Texture::TestScene_Texture() : Scene() {
}


TestScene_Texture::~TestScene_Texture() {
}

void TestScene_Texture::Load() {
	Scene::Load();

	SimpleEngine::SetVsyncMode(false);

	BaseLight* directionalLight = new DirectionalLight();
	AddLight(directionalLight);
	
	mainCamera->transform->position = vec3(20, 20, 160);
	mainCamera->clearColor = vec4(0.8);

	const char* m[] = {
		"suzanne.obj",
		"spot.obj",
		"vase.obj",//2
		"venusm_wNormal.obj",
		"bunny.obj",//4
		"armadillo.obj",
		"sphere.obj",//6
		"hand.obj",
		"sandstone.obj",//8
		"buddha.obj",
		"Sphere/sphere_256_128.obj",//10
		"angel/angel2.obj",
		"bunny_text/bunnyNewunwrap.obj"
	};


	EngineObject* fakeCam = new EngineObject("fakeCam");
	fakeCam->AttachComponent(new FakeCam());
	fakeCam->SetRenderer(new DefaultRenderer());
	fakeCam->GetRenderer()->SetMeshModel(new MeshModel("cube.obj"));
	fakeCam->GetRenderer()->SetShader();
	fakeCam->transform->position = glm::vec3(20, 20, 90);
	fakeCam->transform->scale = glm::vec3(0.1f);
	fakeCam->GetRenderer()->castShadow = false;
	

	//int idx = 11;
	//PreCullingRenderer_Split* srBase = new PreCullingRenderer_Split();
	//srBase->renderMaterial->vertDivision = 8;
	//srBase->renderMaterial->horiDivision = 32;
	//srBase->SetMeshModel(FilePooler::LoadMeshModel(m[idx]));
	//srBase->renderMaterial->targetCamTr = fakeCam->transform;
	//
	//go = new GameObject(m[idx]);
	//go->SetRenderer(srBase);
	//go->GetRenderer()->SetMeshModel(FilePooler::LoadMeshModel(m[idx]));
	//go->GetRenderer()->SetShader();
	//go->GetRenderer()->castShadow = false;
	//go->transform->position = glm::vec3(2, 0, -10);

	//GameObject* norm = new GameObject();
	//norm->SetRenderer(new DefaultRenderer());
	//norm->GetRenderer()->SetMeshModel(new MeshModel(m[idx]));
	//norm->GetRenderer()->SetShader();
	//norm->transform->position = glm::vec3(-2, 0, -10);

	int c = 5;
	int d = 4;
	int e = 5;
	int objIdx = 12;
	bool splitRender = true;

	if (splitRender) {
		PreCullingRenderer_Split* srBase = new PreCullingRenderer_Split();
		srBase->renderMaterial->vertDivision = 4;
		srBase->renderMaterial->horiDivision = 32;
		srBase->SetMeshModel(FilePooler::LoadMeshModel(m[objIdx]));
		srBase->renderMaterial->targetCamTr = mainCamera->transform;
		float dist = srBase->renderMaterial->boundingSphere->radius * 2;
		for (int loop = 0; loop < c; loop++) {
			for (int loop2 = 0; loop2 < d; loop2++) {
				for (int loop3 = 0; loop3 < e; loop3++) {
					EngineObject* go = new EngineObject(m[objIdx]);
					PreCullingRenderer_Split* sr = new PreCullingRenderer_Split();
					sr->renderMaterial = srBase->renderMaterial;
					go->SetRenderer(sr);
					go->GetRenderer()->SetMeshModel(FilePooler::LoadMeshModel(m[objIdx]));
					go->GetRenderer()->SetShader();
					go->GetRenderer()->castShadow = false;
					go->transform->position = glm::vec3(loop * dist, loop2 * dist, loop3 * dist);
				}
			}
		}
	} else {
		float dist = 13;
		for (int loop = 0; loop < c; loop++) {
			for (int loop2 = 0; loop2 < d; loop2++) {
				for (int loop3 = 0; loop3 < e; loop3++) {
					EngineObject* go = new EngineObject(m[objIdx]);
					go->SetRenderer(new DefaultRenderer);
					go->GetRenderer()->SetMeshModel(FilePooler::LoadMeshModel(m[objIdx]));
					go->GetRenderer()->SetShader();
					go->GetRenderer()->castShadow = false;
					go->transform->position = glm::vec3(loop * dist, loop2 * dist, loop3 * dist);
				}
			}
		}
	}
}
