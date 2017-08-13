#include "DirectionalLight.h"



DirectionalLight::DirectionalLight(){
	intensity = 1350.0f;
	color = glm::vec3(0.5f, 0.5f, 1);
	position = glm::vec3(1, 30, 1);
}


DirectionalLight::~DirectionalLight(){

}
