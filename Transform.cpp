#include "Transform.h"

#include <iostream>
#include "SimpleMath.h"

Transform::Transform()
{
	position = glm::vec3(0, 0, 0);
	eulerAngles = glm::vec3(0, 0, 0);
	rotation = quat(eulerAngles);
	scale = glm::vec3(1, 1, 1);
	forward = glm::vec3(0, 0, 1);
	right = glm::vec3(1, 0, 0);
	up = glm::vec3(0, 1, 0);	
}

vec3 Transform::GetEulerAngles() {
	return eulerAngles;
}

void Transform::SetEulerAngles(vec3 rot) {	
	rotation = quat(SimpleMath::DegreeToRadianV3(rot));	
}

vec3 Transform::GetForward() {
	return forward;
}

void Transform::SetForward(vec3 rot) {
	forward = rot;
}

vec3 Transform::GetRight() {
	return right;
}

void Transform::SetRight(vec3 rot) {
	right = rot;
}

vec3 Transform::GetUp() {
	return up;
}

void Transform::SetUp(vec3 rot) {
	up = rot;
}

void Transform::Rotate(vec3 eulerAngles) {
	rotate(rotation, eulerAngles);
}


void Transform::SetParent(Transform * parent_){
	parent = parent_;	
	parent_->children.push_back(this);
}

mat4 Transform::GetMatrix4() {	
	mat4 matTranslation = translate(glm::mat4(1.0), position);;	
	mat4 matRotation = toMat4(rotation);
	mat4 matScale = glm::scale(glm::mat4(1.0), scale);

	return matTranslation * matRotation * matScale;
}

Transform::~Transform()
{
}
