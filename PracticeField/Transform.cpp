#include "Transform.h"

#include <iostream>

Transform::Transform()
{
	position = glm::vec3(0, 0, 0);
	rotation = glm::vec3(0, 0, 0);
	scale = glm::vec3(1, 1, 1);
}


Transform::~Transform()
{
	printf("?? %f", position.x);
}
