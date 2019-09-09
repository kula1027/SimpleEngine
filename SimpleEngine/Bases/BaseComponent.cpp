#include "BaseComponent.h"

void BaseComponent::OnAttachedToObject(EngineObject * obj_) {
	engineObject = obj_;
}

EngineObject * BaseComponent::GetAttachedObject() {
	return engineObject;
}
