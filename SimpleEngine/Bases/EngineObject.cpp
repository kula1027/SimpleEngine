#include "EngineObject.h"

#include "BaseScript.h"
#include <FilePooler.h>
#include <Scene/Scene.h>

unsigned int EngineObject::freeObjectId = 0;

unsigned int EngineObject::GetFreeObjectId() {
	return ++freeObjectId;
}

void EngineObject::Initialize() {
	transform = new Transform();
	transform->engineObject = this;
	Scene::RegisterObject(this);

	isActive = true;
	objectId = GetFreeObjectId();
}

EngineObject::EngineObject(){
	Initialize();
}

EngineObject::EngineObject(std::string name_){
	name = name_;
	Initialize();
}

unsigned int EngineObject::GetObjectId(){
	return objectId;
}


BaseComponent * EngineObject::AttachComponent(BaseComponent* baseComponent_) {
	components.push_back(baseComponent_);
	baseComponent_->OnAttachedToObject(this);

	if (BaseScript* script = dynamic_cast<BaseScript*>(baseComponent_)) {
		script->engineObject = this;
		script->transform = this->transform;
		Scene::RegisterScript(script);

		script->OnStart();
	}

	return baseComponent_;
}

bool EngineObject::GetActiveState() {
	return isActive;
}

void EngineObject::SetActiveState(bool state_) {
	isActive = state_;
}

void EngineObject::NotifyTransformChange() {
	for (int loop = 0; loop < components.size(); loop++) {
		components[loop]->OnTransformChanged();
	}
}

EngineObject::~EngineObject(){	
	free(transform);
	for (int loop = 0; loop < components.size(); loop++) {
		free(components[loop]);
	}
}