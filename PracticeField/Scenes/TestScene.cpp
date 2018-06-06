#include "TestScene.h"
#include "SceneIncludes.h"

TestScene::TestScene() : Scene() {
}


TestScene::~TestScene() {
}

void TestScene::Load() {
	Scene::Load();

	BaseLight* directionalLight = new DirectionalLight();
	AddLight(directionalLight);

	skybox = new EmptySkyBox();

	camera->transform->position = glm::vec3(0, 4, 8);
	camera->clearColor = glm::vec4(0.8);

	GameObject* goTimer = new GameObject("timer");
	goTimer->AddComponent<TimeChecker>();

	GameObject* fakeCam = new GameObject("fakeCam");
	fakeCam->AddComponent<FakeCam>();
	fakeCam->SetRenderer(new Renderer());
	fakeCam->GetRenderer()->SetMeshModel(FileManager::LoadMeshModel("cube.obj"));
	fakeCam->GetRenderer()->SetShader();
	fakeCam->transform->position = glm::vec3(0, 0, -3);
	fakeCam->transform->scale = glm::vec3(0.05f);
	fakeCam->GetRenderer()->castShadow = false;


	char* m[] = {
		"venusm_wNormal.obj",
		"armadillo.obj",
		"bunny.obj",
		"hand.obj",//3
		"suzanne.obj",
		"spot.obj",
		"vase.obj",//6
		"sphere.obj"
	};

	CullLayerRenderer* clRdr = new CullLayerRenderer();	
	clRdr->SetMeshModel(FileManager::LoadMeshModel(m[5]));
	clRdr->SetShader();
	clRdr->refTransform = fakeCam->transform;
	clRdr->castShadow = false;
	GameObject* go = new GameObject();
	go->SetRenderer(clRdr);

	PreCullingColorLayer* srBase = new PreCullingColorLayer();
	srBase->SetMeshModel(FileManager::LoadMeshModel_Pool(m[5]));
	srBase->renderMaterial->targetCamTr = fakeCam->transform;
	srBase->castShadow = false;
	srBase->SetShader(FileManager::LoadShader("vertexColorDiffuse.vert", "vertexColorDiffuse.frag"));
	GameObject* go2 = new GameObject();	
	go2->SetRenderer(srBase);	
	go2->transform->position = glm::vec3(2, 0, 0);

	//int c = 20;
	//int d = 20;
	//int objIdx = 6;
	//PreCullingRenderer_Split* srBase = new PreCullingRenderer_Split();
	//srBase->SetMeshModel(FileManager::LoadMeshModel_Pool(m[objIdx]));
	//srBase->renderMaterial->targetCamTr = fakeCam->transform;
	//for (int loop = 0; loop < c; loop++) {
	//	for (int loop2 = 0; loop2 < d; loop2++) {
	//		GameObject* go = new GameObject(m[objIdx]);
	//		PreCullingRenderer_Split* sr = new PreCullingRenderer_Split();
	//		sr->renderMaterial = srBase->renderMaterial;
	//		go->SetRenderer(sr);
	//		go->GetRenderer()->SetMeshModel(FileManager::LoadMeshModel_Pool(m[objIdx]));
	//		go->GetRenderer()->SetShader();
	//		go->GetRenderer()->castShadow = false;
	//		go->transform->position = glm::vec3(loop * 10 - c * 5, 0, -loop2 * 10);
	//		//go->transform->position = glm::vec3(0, 0, 0);
	//	}
	//}
}
