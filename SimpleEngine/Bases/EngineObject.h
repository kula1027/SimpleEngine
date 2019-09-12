#pragma once

#define strNoname "noname"

#include "Transform.h"
#include "../Scenes/Scene.h"

#include <vector>
#include <string>
#include <glm/gtc/matrix_transform.hpp>
#include <Debugger/SP_Debugger.h>

class MeshRenderer;
class BaseLight;

class BaseComponent;

class EngineObject {
private:
	unsigned int objectId;
	std::vector<BaseComponent*> components;

	void Initialize();

public:
	std::string name = strNoname;
	Transform* transform = NULL;

	EngineObject();
	~EngineObject();
	EngineObject(std::string name_);

	void SetId(unsigned int id_);
	unsigned int GetId();

	BaseComponent* AttachComponent(BaseComponent*);

	template <class T>
	T* GetComponent();
};

template<class T>
T * EngineObject::GetComponent(){
	for (int loop = 0; loop < components.size(); loop++) {
		if (T* component = dynamic_cast<T*>(components[loop])) {
			return component;
		}
	}

	return NULL;
}
