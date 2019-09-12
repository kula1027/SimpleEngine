#include "BaseLight.h"
#include <Scenes/Scene.h>

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

void BaseLight::SetPosition(glm::vec3 position_) {
	position = position_;
	uniformValueHasChanged = true;
}

glm::vec3 BaseLight::GetPosition() {
	return position;
}

void BaseLight::OnAttachedToObject(EngineObject * obj_) {
	BaseComponent::OnAttachedToObject(obj_);

	Scene::RegisterLight(this);
}
