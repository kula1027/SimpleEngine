#include "PointLight.h"



PointLight::PointLight(){
	intensity = 150.0f;
	color = glm::vec3(1, 1, 1);
	range = 10;
}


PointLight::~PointLight()
{
}

void PointLight::Update(){
	
}

void PointLight::Use(Shader * shader){

}
