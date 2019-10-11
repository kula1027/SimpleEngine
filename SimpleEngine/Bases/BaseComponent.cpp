#include "BaseComponent.h"
#include "EngineObject.h"

unsigned int BaseComponent::freeComponentId = 0;

unsigned int BaseComponent::GetFreeComponentId(){
	return ++freeComponentId;
}

BaseComponent::BaseComponent(){
	freeComponentId = GetFreeComponentId();	
}

void BaseComponent::OnAttachedToObject(EngineObject * obj_) {
	engineObject = obj_;
	transform = engineObject->transform;

	engineObject->NotifyTransformChange();
}

EngineObject * BaseComponent::GetEngineObject() {
	return engineObject;
}

Transform * BaseComponent::GetTransform() {
	return transform;
}

unsigned int BaseComponent::GetComponentId() {
	return componentId;
}

BaseComponent::~BaseComponent() {
}

void BaseComponent::SetEnabled(bool enabled_) {
	isEnabled = enabled_;
}

bool BaseComponent::GetEnabled() {
	return (engineObject->GetActiveState() && isEnabled);
}
