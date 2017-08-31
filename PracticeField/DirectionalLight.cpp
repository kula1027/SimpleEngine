#include "DirectionalLight.h"
#include <glm\gtc\matrix_transform.hpp>


DirectionalLight::DirectionalLight(){
	intensity = 0.8f;
	color = glm::vec3(1, 1, 1);
	position = glm::vec3(0, -1, -1);

	glm::mat4 lightProjection = glm::ortho(-100.0f, 100.0f, -100.0f, 100.0f, shadowData.nearPlane, shadowData.farPlane);
	glm::mat4 lightView = glm::lookAt(
		glm::vec3(-2.0f, 100.0f, -1.0f),
		glm::vec3(0.0f, 0.0f, 0.0f),
		glm::vec3(0.0f, 1.0f, 0.0f));
	glm::mat4 lightSpaceMatrix = lightProjection * lightView;
}


DirectionalLight::~DirectionalLight(){

}
