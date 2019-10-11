#include "PointLight.h"
#include <Bases/Transform.h>
#include <glm\glm.hpp>


PointLight::PointLight(){
	lightType = LightType_Point;

	intensity = 10;
	color = glm::vec3(0.5f, 0.5f, 1);
	range = 20;	
}


PointLight::~PointLight(){
}

glm::mat4 PointLight::GetModelMatrix() {
	return modelMatrix;
}

void PointLight::OnTransformChanged() {
	mat4 matTranslation = translate(mat4(1.0), GetTransform()->position);
	mat4 matScale = mat4(range);	
	modelMatrix = matTranslation * matScale;
}

