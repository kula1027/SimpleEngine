#include "GameObject.h"

#include "Renderer.h"
#include "FileLoader.h"
#include "Scene.h"

GameObject::GameObject(){
	name = strNoname;	
	Scene::GetCurrent()->objectPool.AddGameObject(this);
}

void GameObject::SetRenderer(Renderer * renderer_){
	renderer_->SetReferences(&transform, meshModel);

	Scene::GetCurrent()->objectPool.AddRenderer(renderer_);
	renderer = renderer_;
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
