#include "TestScene3.h"
#include "SceneIncludes.h"


TestScene3::TestScene3() : Scene() {
}


TestScene3::~TestScene3() {
}

void TestScene3::Load() {
	Scene::Load();

	SimpleEngine::SetVsyncMode(true);

	mainCamera->transform->position = glm::vec3(0, 0, -3);
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
	fakeCam->transform->position = glm::vec3(-1, 0, 0);
	fakeCam->transform->scale = glm::vec3(0.1f);
	fakeCam->GetRenderer()->castShadow = false;

	char* m[] = {
		"suzanne.obj",
		"spot.obj",
		"vase.obj",//2
		"venusm_wNormal.obj",
		"bunny.obj",//4
		"armadillo.obj",
		"sphere.obj",//6
		"hand.obj",
		"sandstone.obj",//8
		"buddha.obj"
	};

	/*CullLayerRenderer* clRdr = new CullLayerRenderer();	
	clRdr->SetMeshModel(FilePooler::LoadMeshModel(m[3]));
	clRdr->SetShader();
	clRdr->refTransform = fakeCam->transform;
	clRdr->castShadow = false;
	GameObject* go = new GameObject();
	go->SetRenderer(clRdr);
	go->transform->position = glm::vec3(0, 0, 0);
*/

	PreCullingColorLayer* srBase = new PreCullingColorLayer();
	srBase->renderMaterial->horiDivision = 128;
	srBase->renderMaterial->vertDivision = 16;
	srBase->SetMeshModel(FilePooler::LoadMeshModel(m[6]));
	srBase->renderMaterial->targetCamTr = fakeCam->transform;
	srBase->SetShader(FilePooler::LoadShader("vertexColorDiffuse.vert", "vertexColorDiffuse.frag"));
	GameObject* go2 = new GameObject();
	go2->SetRenderer(srBase);
	go2->transform->position = glm::vec3(0, 0, 0);

	//GameObject* norm = new GameObject();
	//norm->SetRenderer(new DefaultRenderer());
	//norm->GetRenderer()->SetMeshModel(FilePooler::LoadMeshModel(m[4]));
	//norm->GetRenderer()->SetShader();
	//go->transform->position = glm::vec3(-4, 0, 0);

/*
	PreCullingRenderer_Split* rdrSplit = new PreCullingRenderer_Split();
	rdrSplit->SetMeshModel(FilePooler::LoadMeshModel(m[6]));
	rdrSplit->renderMaterial->targetCamTr = fakeCam->transform;
	rdrSplit->SetShader();
	GameObject* go3 = new GameObject();
	go3->SetRenderer(rdrSplit);
	go3->transform->position = glm::vec3(0, 0, 0);*/
}


