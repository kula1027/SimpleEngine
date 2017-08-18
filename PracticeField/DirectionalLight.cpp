#include "DirectionalLight.h"



DirectionalLight::DirectionalLight(){
	intensity = 0.8f;
	color = glm::vec3(1, 1, 1);
	position = glm::vec3(0, -1, -1);
}


DirectionalLight::~DirectionalLight(){

}
