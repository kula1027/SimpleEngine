#include "PointLight.h"



PointLight::PointLight(){
	intensity = 400.0f;
	color = glm::vec3(0.1f, 1, 1);
	range = 350;
	position = glm::vec3(30, 50, 0);
}


PointLight::~PointLight(){
}

void PointLight::Update(){
	//position.x += Time::deltaTime;
}

void PointLight::Use(Shader * shader){

}
