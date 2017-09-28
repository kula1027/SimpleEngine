#include "Transform.h"

#include <iostream>

Transform::Transform()
{
	position = glm::vec3(0, 0, 0);
	rotation = glm::vec3(0, 0, 0);
	scale = glm::vec3(1, 1, 1);
}


void Transform::SetParent(Transform * parent_){
	parent = parent_;
	parent_->children.push_back(this);
}

Transform::~Transform()
{
}
