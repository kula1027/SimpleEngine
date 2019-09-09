#pragma once

#define strNoname "noname"

#include "Transform.h"
#include "../Scenes/Scene.h"

#include <vector>
#include <string>
#include <glm/gtc/matrix_transform.hpp>

class BaseRenderer;
class BaseLight;

class BaseComponent;

class EngineObject {
private:	
	unsigned int objectId;
	BaseRenderer* renderer;
	std::vector<BaseComponent*> components;

public:
	std::string name = strNoname;
	Transform* transform = NULL;

	EngineObject();
	~EngineObject();
	EngineObject(std::string name_);

	void SetId(unsigned int id_);
	unsigned int GetId();
	void SetRenderer(BaseRenderer* renderer_);
	BaseRenderer* GetRenderer();

	template <class T>
	T* AddComponent();

	template <class T>
	T* GetComponent();
};

template<class T> 
T* EngineObject::AddComponent(){
	T* t = new T;
	if (BaseScript* upd = dynamic_cast< BaseScript* >(t)) {
		upd->gameObject = this;
		upd->transform = this->transform;
		Scene::GetCurrent()->AddScript(upd);
		components.push_back(t);

		upd->OnStart();
	}

	return t;
}

template<class T>
T * EngineObject::GetComponent(){
	for (int loop = 0; loop < components.size(); loop++) {
		if (T* component = dynamic_cast<T*>(components[loop])) {
			return component;
		}
	}

	return NULL;
}
