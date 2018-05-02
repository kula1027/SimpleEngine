#include "GameObject.h"

#include "IUpdatable.h"
#include "Renderer.h"
#include "FileManager.h"
#include "Scene.h"
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

void GameObject::SetId(unsigned int id_){
	objectId = id_;
}

unsigned int GameObject::GetId(){
	return objectId;
}

void GameObject::SetRenderer(Renderer * renderer_){
	renderer_->SetTransform(transform);

	Scene::GetCurrent()->AddRenderer(renderer_);
	renderer = renderer_;
}

Renderer * GameObject::GetRenderer(){
	return renderer;
}


GameObject::~GameObject(){	
	free(renderer);
	free(transform);
	for (int loop = 0; loop < components.size(); loop++) {
		free(components[loop]);
	}
}