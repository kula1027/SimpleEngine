#include "Scene.h"


Scene::Scene(){

}


Scene::~Scene(){
	
}

void Scene::Load() {
	int that = 10;
	for (int loop = -that; loop < that; loop++) {
		for (int loop2 = -that; loop2 < that; loop2++) {
			GameObject* go = new GameObject();

			MeshModel* meshModel = LoadMesh("Sphere.obj");
			go->SetModel(meshModel);
			go->SetShader(FileLoader::LoadShader("shaderVertex.shader", "shaderFragment.fragment"));
			go->transform.position = glm::vec3(loop2 * 4, loop * 4, 0);
			objectPool.AddGameObject(go);
		}
	}

	Light* pointLight = new PointLight();
	objectPool.AddLight(pointLight);
}

MeshModel * Scene::LoadMesh(string path){	
	MeshModel* foundMesh = NULL;

	for (int loop = 0; loop < meshModelPool.size(); loop++) {
		if (meshModelPool[loop]->GetDirectory().compare(path.c_str())) {			
			foundMesh = meshModelPool[loop];
			break;
		}
	}

	if (foundMesh != NULL) {
		return foundMesh;
	}else {
		MeshModel* meshModel = FileLoader::LoadMeshModel(path);
		meshModelPool.push_back(meshModel);

		return meshModel;
	}
}

Shader * Scene::LoadShader(){
	return nullptr;
}


void Scene::UpdateObjects(){
	camera.Update();

	objectPool.UpdateObjects();
}

void Scene::RenderObjects(){
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	objectPool.RenderObjects(&camera);
}
