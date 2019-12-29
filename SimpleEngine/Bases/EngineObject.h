#pragma once

#define strNoname "noname"

#include "Transform.h"
#include <Scene/Scene.h>

#include <vector>
#include <string>
#include <glm/gtc/matrix_transform.hpp>
#include <Debugger/SP_Debugger.h>

class MeshRenderer;
class BaseLight;

class BaseComponent;

class EngineObject {
private:
	static unsigned int freeObjectId;
	unsigned int GetFreeObjectId();

	unsigned int objectId;
	std::vector<BaseComponent*> components;
	Transform* transform = NULL;

	void Initialize();

	bool isActive;

public:
	std::string name = strNoname;	

	static void Destroy(EngineObject* engineObject_);

	EngineObject();
	~EngineObject();
	EngineObject(std::string name_);

	unsigned int GetObjectId();

	template <class T>
	T* GetComponent();
	BaseComponent* AttachComponent(BaseComponent*);

	Transform* GetTransform();

	bool GetActiveState();
	void SetActiveState(bool state_);	

	void OnTransformChange();

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
