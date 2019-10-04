#include "BaseComponent.h"
#include "EngineObject.h"


void BaseComponent::OnAttachedToObject(EngineObject * obj_) {
	engineObject = obj_;
	transform = engineObject->transform;
}

EngineObject * BaseComponent::GetEngineObject() {
	return engineObject;
}

Transform * BaseComponent::GetTransform() {
	return transform;
}

BaseComponent::~BaseComponent() {
}
