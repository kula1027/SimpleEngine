#include "PointLight.h"
#include <Bases/Transform.h>


PointLight::PointLight(){
	intensity = 200;
	color = glm::vec3(0.5f, 0.5f, 1);
	range = 350;
	GetTransform()->position = glm::vec3(30, 15, 0);
}


PointLight::~PointLight(){
}

void PointLight::Update(){
	//position.x += Time::deltaTime;
}

