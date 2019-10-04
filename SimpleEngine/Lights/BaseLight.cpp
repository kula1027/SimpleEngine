#include "BaseLight.h"
#include <Lights/LightManager.h>

BaseLight::BaseLight(){
}


BaseLight::~BaseLight()
{
}

void BaseLight::SetIntensity(float intensity_) {
	intensity = intensity_;
	uniformValueHasChanged = true;
}

float BaseLight::GetIntensity() {
	return intensity;
}

void BaseLight::SetColor(glm::vec3 color_) {
	color = color_;
	uniformValueHasChanged = true;
}

glm::vec3 BaseLight::GetColor() {
	return color;
}

void BaseLight::OnAttachedToObject(EngineObject * obj_) {
	BaseComponent::OnAttachedToObject(obj_);

	LightManager::Inst()->AddLight(this);	
}
