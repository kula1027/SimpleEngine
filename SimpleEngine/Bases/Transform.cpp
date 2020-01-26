#include "Transform.h"

#include <iostream>
#include <Bases/EngineObject.h>
#include <Util/SimpleMath.h>


//현재 붙어있는 engineObject의 다른 컴포넌트들에게 변경되었음을 알림
void Transform::OnChange() {
	nuModelMatrix = true;
	engineObject->OnTransformChange();
}

void Transform::CalcModelMatrix() {
	mat4 matTranslation = translate(glm::mat4(1.0), position);
	mat4 matRotation = toMat4(rotation);
	mat4 matScale = glm::scale(glm::mat4(1.0), scale);

	modelMatrix = matTranslation * matRotation * matScale;	

	nuModelMatrix = false;
}

Transform::Transform(EngineObject* engineObject_) {
	engineObject = engineObject_;

	position = glm::vec3(0, 0, 0);
	eulerAngles = glm::vec3(0, 0, 0);
	rotation = quat(eulerAngles);
	scale = glm::vec3(1, 1, 1);
	forward = glm::vec3(0, 0, 1);
	right = glm::vec3(1, 0, 0);
	up = glm::vec3(0, 1, 0);
	
	CalcModelMatrix();
}

vec3 Transform::GetPosition() {
	return position;
}

void Transform::SetPosition(vec3 position_) {
	position = position_;
	OnChange();
}

void Transform::AddPosition(vec3 addVec3) {
	SetPosition(position + addVec3);
}

vec3 Transform::GetScale() {
	return scale;
}

void Transform::SetScale(vec3 scale_) {
	scale = scale_;
	OnChange();	
}

int Transform::GetChildCount() {
	return children.size();
}

Transform * Transform::GetChildAt(int idx_) {
	return children.at(idx_);
}

void Transform::AddChild(Transform * transform_) {
	children.push_back(transform_);
}

void Transform::RemoveChildAt(int idx_) {
	children.erase(children.begin() + idx_);
}

vec3 Transform::GetEulerAngles() {
	return eulerAngles;
}

void Transform::SetEulerAngles(vec3 eulerAngles_) {
	rotation = quat(SimpleMath::DegreeToRadianV3(eulerAngles_));
}

vec3 Transform::GetForward() {
	return forward;
}

void Transform::SetForward(vec3 eulerAngles_) {
	forward = eulerAngles_;
	OnChange();
}

vec3 Transform::GetRight() {
	return right;
}

void Transform::SetRight(vec3 eulerAngles_) {
	right = eulerAngles_;
}

vec3 Transform::GetUp() {
	return up;
}

void Transform::SetUp(vec3 eulerAngles_) {
	up = eulerAngles_;
}

void Transform::Rotate(vec3 eulerAngles_) {
	rotate(rotation, eulerAngles);
}


EngineObject * Transform::GetEngineObject() {
	return engineObject;
}

Transform * Transform::GetParent() {
	return parent;
}

void Transform::SetParent(Transform * parent_) {
	parent = parent_;
	parent_->children.push_back(this);
}

mat4 Transform::GetModelMatrix() {
	if (nuModelMatrix) {
		CalcModelMatrix();
	}

	return modelMatrix;
}

Transform::~Transform() {
}
