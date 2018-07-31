#include "GameObject.h"

#include "IUpdatable.h"
#include "../Render/DefaultRenderer.h"
#include "../FilePooler.h"
#include "../Scenes/Scene.h"
#include <typeinfo>

GameObject::GameObject(){
	transform = new Transform();
	transform->gameObject = this;
	Scene::GetCurrent()->AddGameObject(this);	
}

GameObject::GameObject(std::string name_){
	name = name_;
	transform = new Transform();
	transform->gameObject = this;
	Scene::GetCurrent()->AddGameObject(this);
}

void GameObject::Initialize() {
	for (int loop = 0; loop < components.size(); loop++) {
		components[loop]->Initialize();
	}
}

void GameObject::SetId(unsigned int id_){
	objectId = id_;
}

unsigned int GameObject::GetId(){
	return objectId;
}

void GameObject::SetRenderer(BaseRenderer * renderer_){
	renderer_->SetTransform(transform);

	Scene::GetCurrent()->AddRenderer(renderer_);
	renderer = renderer_;
}

BaseRenderer * GameObject::GetRenderer(){
	return renderer;
}


GameObject::~GameObject(){	
	free(renderer);
	free(transform);
	for (int loop = 0; loop < components.size(); loop++) {
		free(components[loop]);
	}
}