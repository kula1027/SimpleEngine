#pragma once

#define strNoname "noname"

#include "Transform.h"
#include "Scene.h"

#include <vector>
#include <string>
#include <glm/gtc/matrix_transform.hpp>

class Renderer;
class BaseLight;

class IComponent;

class GameObject {
private:	
	unsigned int objectId;
	Renderer* renderer;
	std::vector<IComponent*> components;

public:
	std::string name = strNoname;
	Transform* transform = NULL;

	GameObject();
	~GameObject();
	GameObject(std::string name_);

	void SetId(unsigned int id_);
	unsigned int GetId();
	void SetRenderer(Renderer* renderer_);
	Renderer* GetRenderer();

	template <class T>
	T* AddComponent();

	template <class T>
	T* GetComponent();
};

template<class T> 
T* GameObject::AddComponent(){
	T* t = new T;
	if (IUpdatable* upd = dynamic_cast< IUpdatable* >(t)) {
		upd->gameObject = this;
		upd->transform = this->transform;
		Scene::GetCurrent()->AddUpdatable(upd);
		components.push_back(t);

		upd->OnStart();
	}

	return t;
}

template<class T>
T * GameObject::GetComponent(){
	for (int loop = 0; loop < components.size(); loop++) {
		if (T* component = dynamic_cast<T*>(components[loop])) {
			return component;
		}
	}

	return NULL;
}
