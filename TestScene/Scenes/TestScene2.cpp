#include "TestScene2.h"

#include <Scenes/SceneIncludes.h>


TestScene2::TestScene2() {
}


TestScene2::~TestScene2() {
}

void TestScene2::Load() {
	Scene::Load();

	mainCamera->transform->position = glm::vec3(0, 10, 20);

	EngineObject* eoLight = new EngineObject("light");
	DirectionalLight* directionalLight = new DirectionalLight();
	eoLight->AttachComponent(directionalLight);

	EngineObject* eo = new EngineObject("nano");
	MeshRenderer* dRdr = new MeshRenderer();
	eo->AttachComponent(dRdr);
	dRdr->SetMeshModel(FilePooler::LoadMeshModel("nanosuit/nanosuit.obj"));
	dRdr->SetShader();
	eo->transform->position = glm::vec3(0, 0, 0);

	EngineObject* sph = new EngineObject("bunny");
	MeshRenderer* dRdr2 = new MeshRenderer();
	sph->AttachComponent(dRdr2);
	dRdr2->SetMeshModel(FilePooler::LoadMeshModel("bunny.obj"));
	dRdr2->SetShader();
	sph->transform->position = glm::vec3(0, -1, 0);


}