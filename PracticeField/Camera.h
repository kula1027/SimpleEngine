#pragma once
#include <glm\glm.hpp>
#include <glm\gtc\matrix_transform.hpp>

class Transform;

#define PROJECTION_ORTHO 0;
#define PROJECTION_PERSPECTIVE 1;

class Camera
{
private:
	float fov;
	float near;
	float far;

	float verticalAngle;
	float horizontalAngle;
	glm::vec3 dirForward;
	glm::vec3 right;

	glm::vec3 upVector;

	glm::mat4 viewMatrix;
	glm::mat4 projectionMatrix;
	glm::mat4 mvpMatrix;

	int projMode;

	float moveSpeed;
	float sensitivity;

	void ComputeMatrix();

public:
	Camera();
	~Camera();
	glm::mat4 VPmatrix();
	glm::mat4 Vmatrix();
	void Update();

	Transform* transform;

};

