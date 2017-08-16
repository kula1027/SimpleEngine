#include "GameObject.h"

GameObject::GameObject(){
	name = new string(strNoname);
	renderer = new Renderer(&transform);
}

void GameObject::Render(Camera* cam, std::vector<Light*> lights_){
	renderer->Render(cam, lights_, meshModel);
}

void GameObject::Update(){
	//transform.position.x += Time::deltaTime;
}

void GameObject::SetModel(MeshModel * meshModel_) {
	meshModel = meshModel_;	
}

void GameObject::SetShader(Shader * shader_) {
	renderer->SetShader(shader_);
}


GameObject::~GameObject(){	
	free(meshModel);
	free(renderer);
	free(name);
}
