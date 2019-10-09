#include "BaseLight.h"
#include <Lights/LightManager.h>

BaseLight::BaseLight(){
}


BaseLight::~BaseLight()
{
}

void BaseLight::SetIntensity(float intensity_) {
	intensity = intensity_;	
}

float BaseLight::GetIntensity() {
	return intensity;
}

void BaseLight::SetColor(glm::vec3 color_) {
	color = color_;	
}

glm::vec3 BaseLight::GetColor() {
	return color;
}

LightType BaseLight::GetLightType() {
	return lightType;
}

void BaseLight::SetStartAddrUbo(int startAddr_) {
	startAddrUbo = startAddr_;
}

void BaseLight::OnAttachedToObject(EngineObject * obj_) {
	BaseComponent::OnAttachedToObject(obj_);

	LightManager::Inst()->AddLight(this);	
}
