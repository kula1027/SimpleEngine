#include "PointLight.h"



PointLight::PointLight(){
	intensity = 150.0f;
	color = glm::vec3(1, 1, 1);
	range = 10;
	position = glm::vec3(0, 10, 0);
}


PointLight::~PointLight(){
}

void PointLight::Update(){
	//position.x += Time::deltaTime;
}

void PointLight::Use(Shader * shader){

}
