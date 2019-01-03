#include "Sc_ColorLayer.h"
#include <Scenes/SceneIncludes.h>
#include "Renderer/PreCullingColorLayer.h"


Sc_ColorLayer::Sc_ColorLayer() : Scene() {
}


Sc_ColorLayer::~Sc_ColorLayer() {
}

void Sc_ColorLayer::Load() {
	Scene::Load();

	SimpleEngine::SetVsyncMode(true);

	mainCamera->transform->position = glm::vec3(0, 0, -7);
	mainCamera->clearColor = glm::vec4(1.0f);
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
	glm::vec3 tPos = normalize(glm::vec3(-1, 0, 0));
	tPos *= 4;
	fakeCam->transform->position = glm::vec3(tPos.x, 0, tPos.z);
	fakeCam->transform->scale = glm::vec3(0.03f);
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
		"angel2.obj"//10
	};

	/*CullLayerRenderer* clRdr = new CullLayerRenderer();	
	clRdr->SetMeshModel(FilePooler::LoadMeshModel("Sphere/sphere_1024_512.obj"));
	clRdr->SetShader();
	clRdr->refTransform = fakeCam->transform;
	clRdr->castShadow = false;
	GameObject* go = new GameObject();
	go->SetRenderer(clRdr);
	go->transform->position = glm::vec3(0, 0, 0);*/


	PreCullingColorLayer* srBase = new PreCullingColorLayer();
	srBase->renderMaterial->horiDivision = 128;
	srBase->renderMaterial->vertDivision = 32;
	srBase->SetMeshModel(new MeshModel("bunny.obj"));
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


