#include "TestScene2.h"

#include <Scenes/SceneIncludes.h>


TestScene2::TestScene2() {
}


TestScene2::~TestScene2() {
}

void TestScene2::Load() {
	Scene::Load();

	//mainCamera->renderMode = RenderPath_Deferred;

	BaseLight* directionalLight = new DirectionalLight();
	AddLight(directionalLight);
	
	mainCamera->transform->position = glm::vec3(0, 10, 0);

	/*EngineObject* go = new EngineObject("nano");
	go->SetRenderer(new DefaultRenderer());
	go->GetRenderer()->SetMeshModel(FilePooler::LoadMeshModel("nanosuit/nanosuit.obj"));
	go->GetRenderer()->SetShader();
	go->transform->position = glm::vec3(0, 0, -30);*/
}