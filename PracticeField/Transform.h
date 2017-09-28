#pragma once

#include <glm\glm.hpp>
#include <vector>

class GameObject;

class Transform
{
public:
	Transform();
	glm::vec3 position;
	glm::vec3 rotation;
	glm::vec3 scale;

	GameObject* gameObject;

	std::vector<Transform*> children;
	Transform* parent = NULL;

	void SetParent(Transform* parent_);
	~Transform();
};

