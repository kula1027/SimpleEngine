#include "PointLight.h"
#include <Bases/Transform.h>
#include <gl\glew.h>
#include <glm\glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <Lights/LightManager.h>

void PointLight::SetUboIntensity() {
	float inten = GetIntensity();
	glBufferSubData(GL_UNIFORM_BUFFER,
		startAddrUbo,
		sizeof(float),
		&inten);
}
//Dist	Constant	Linear	Quadratic
//100	1.0			0.045	0.0075
void PointLight::SetUboRange() {
	vec4 attenK_c_l_q_range = vec4(
		1.0 ,	//constant
		0.045f,		//linear
		0.0075f,		//quadratic
		range	//Range
		);

	glBufferSubData(GL_UNIFORM_BUFFER,
		startAddrUbo + sizeof(glm::vec4) * 3,
		sizeof(glm::vec4),
		&attenK_c_l_q_range);
}

PointLight::PointLight(){
	lightType = LightType_Point;
	
	color = glm::vec3(0, 0.5f, 1);		
}


PointLight::~PointLight(){
}

void PointLight::SetRange(float range_) {
	range = range_;

	matScale = scale(mat4(1.0), vec3(range));

	modelMatrix = matTranslation * matScale;

	SetUboRange();
}

float PointLight::GetRange() {
	return range;	
}

glm::mat4 PointLight::GetModelMatrix() {
	return modelMatrix;
}

void PointLight::SetUniformsUbo() {
	LightManager::Inst()->BindUboLightData();

	// 0 - 4
	SetUboIntensity();		

	//position 16 - 32
	glBufferSubData(GL_UNIFORM_BUFFER,
		startAddrUbo + sizeof(glm::vec4),
		sizeof(glm::vec4),
		&(GetTransform()->GetPosition()));
	//color 32 - 48
	glBufferSubData(GL_UNIFORM_BUFFER,
		startAddrUbo + sizeof(glm::vec4) * 2,
		sizeof(glm::vec4),
		&color);	

	SetUboRange();
}

void PointLight::OnTransformChanged() {
	matTranslation = translate(mat4(1.0), GetTransform()->position);	
	modelMatrix = matTranslation * matScale;

	SetUniformsUbo();
}

void PointLight::OnAttachedToObject(EngineObject * obj_) {
	BaseLight::OnAttachedToObject(obj_);

	matTranslation = translate(mat4(1.0), GetTransform()->position);
	matScale = scale(mat4(1.0), vec3(range));

	modelMatrix = matTranslation * matScale;	
}

