#include "Sc_SpeedTest.h"

#include <Scenes/SceneIncludes.h>

Sc_SpeedTest::Sc_SpeedTest() : Scene() {
}


Sc_SpeedTest::~Sc_SpeedTest() {
}

void Sc_SpeedTest::Load() {
	Scene::Load();

	SimpleEngine::SetVsyncMode(false);

	mainCamera->transform->position = glm::vec3(6, 8, -80);
	mainCamera->clearColor = glm::vec4(0.8f);
	//mainCamera->renderMode = RenderPath_SimpleSingle;


	BaseLight* directionalLight = new DirectionalLight();
	directionalLight->isShadowCaster = false;
	AddLight(directionalLight);
	
	//GameObject* goTimer = new GameObject("timer");
	//goTimer->AddComponent<TimeChecker>();

	GameObject* fakeCam = new GameObject("fakeCam");
	fakeCam->AddComponent<FakeCam>();
	fakeCam->SetRenderer(new DefaultRenderer());
	fakeCam->GetRenderer()->SetMeshModel(FilePooler::LoadMeshModel("cube.obj"));
	fakeCam->GetRenderer()->SetShader();
	fakeCam->transform->position = glm::vec3(0, 5, 0);
	fakeCam->transform->scale = glm::vec3(0.1f);
	fakeCam->GetRenderer()->castShadow = false;


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
		"Sphere/sphere_256_128.obj"
	};	

	const char* sp[] = {
		"Sphere/sphere_32_16.obj",
		"Sphere/sphere_64_32.obj",
		"Sphere/sphere_128_64.obj",
		"Sphere/sphere_256_128.obj",//3
		"Sphere/sphere_512_256.obj",
		"Sphere/sphere_1024_512.obj"
	};

	//CullLayerRenderer* clRdr = new CullLayerRenderer();	
	//clRdr->SetMeshModel(FileManager::LoadMeshModel(m[0]));	
	//clRdr->SetShader();
	//clRdr->refTransform = fakeCam->transform;
	//clRdr->castShadow = false;
	//GameObject* go = new GameObject();
	//go->SetRenderer(clRdr);
	//go->transform->position = glm::vec3(0, 0, 0);

	/*GameObject* norm = new GameObject();
	norm->SetRenderer(new Renderer());
	norm->GetRenderer()->SetMeshModel(FileManager::LoadMeshModel(m[5]));
	norm->GetRenderer()->SetShader();*/


	//PreCullingColorLayer* srBase = new PreCullingColorLayer();
	//srBase->SetMeshModel(FileManager::LoadMeshModel(m[0]));
	//srBase->renderMaterial->targetCamTr = fakeCam->transform;
	//srBase->castShadow = false;
	//srBase->SetShader(FileManager::LoadShader("vertexColorDiffuse.vert", "vertexColorDiffuse.frag"));
	//GameObject* go2 = new GameObject();	
	//go2->SetRenderer(srBase);	
	//go2->transform->position = glm::vec3(0, 0, 0);

	//PreCullingRenderer_Split* rdrSplit = new PreCullingRenderer_Split();
	//rdrSplit->SetMeshModel(FileManager::LoadMeshModel(m[5]));
	//rdrSplit->renderMaterial->targetCamTr = fakeCam->transform;
	//rdrSplit->castShadow = false;
	//rdrSplit->SetShader();
	//GameObject* go3 = new GameObject();
	//go3->SetRenderer(rdrSplit);
	//go3->transform->position = glm::vec3(-2, 0, 0);

	//for (int loop = 0; loop < 4; loop++) {
	//	GameObject* go = new GameObject(m[loop]);
	//	PreCullingRenderer_Split* sr = new PreCullingRenderer_Split();
	//	sr->renderMaterial->vertDivision = 4;
	//	sr->SetMeshModel(FileManager::LoadMeshModel_Pool(sp[loop]));
	//	sr->renderMaterial->targetCamTr = fakeCam->transform;

	//	sr->renderMaterial = sr->renderMaterial;
	//	go->SetRenderer(sr);
	//	go->GetRenderer()->SetMeshModel(FileManager::LoadMeshModel_Pool(sp[loop]));
	//	go->GetRenderer()->SetShader();
	//	go->GetRenderer()->castShadow = false;
	//	go->transform->position = glm::vec3(loop * 2, 0, 0);
	//}

	int c = 5;
	int d = 4;
	int e = 5;
	int objIdx = 5;

	bool splitRender = true;

	if (splitRender) {
		PreCullingRenderer_Split* srBase = new PreCullingRenderer_Split();
		srBase->renderMaterial->vertDivision = 8;
		srBase->renderMaterial->horiDivision = 32;
		srBase->SetMeshModel(FilePooler::LoadMeshModel(m[objIdx]));
		srBase->renderMaterial->targetCamTr = mainCamera->transform;
		for (int loop = 0; loop < c; loop++) {
			for (int loop2 = 0; loop2 < d; loop2++) {
				for (int loop3 = 0; loop3 < e; loop3++) {
					GameObject* go = new GameObject(m[objIdx]);
					PreCullingRenderer_Split* sr = new PreCullingRenderer_Split();
					sr->renderMaterial = srBase->renderMaterial;
					go->SetRenderer(sr);
					go->GetRenderer()->SetMeshModel(FilePooler::LoadMeshModel(m[objIdx]));
					go->GetRenderer()->SetShader();
					go->GetRenderer()->castShadow = false;
					go->transform->position = glm::vec3(loop * 4, loop2 * 4, loop3 * 4);					
				}
			}
		}
	}else {
		for (int loop = 0; loop < c; loop++) {
			for (int loop2 = 0; loop2 < d; loop2++) {
				for (int loop3 = 0; loop3 < e; loop3++) {
					GameObject* go = new GameObject(m[objIdx]);
					go->SetRenderer(new DefaultRenderer);
					go->GetRenderer()->SetMeshModel(FilePooler::LoadMeshModel(m[objIdx]));
					go->GetRenderer()->SetShader();
					go->GetRenderer()->castShadow = false;
					go->transform->position = glm::vec3(loop * 4, loop2 * 4, loop3 * 4);
				}
			}
		}
	}	
}
