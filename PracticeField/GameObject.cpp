#include "GameObject.h"

#include "Renderer.h"
#include "FileLoader.h"


GameObject::GameObject(){
	name = strNoname;
	renderer = new Renderer(&transform);
}

void GameObject::Render(Camera* cam, std::vector<BaseLight*> lights_){
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
}
