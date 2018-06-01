#pragma once

#include "Bases/BasesBundle.h"
#include <glm\glm.hpp>

class Camera;

class MoveCamera : public IUpdatable {
private:
	float moveSpeed;
	float sensitivity;
	float verticalAngle;
	float horizontalAngle;

	Camera* cam;

public:
	MoveCamera();
	~MoveCamera();
	virtual void OnUpdate();
	virtual void OnStart();
};

