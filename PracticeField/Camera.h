#pragma once
#include <gl\glew.h>
#include <glm\glm.hpp>
#include <glm\gtc\matrix_transform.hpp>

class Transform;
class Shader;

#define PROJECTION_ORTHO 0;
#define PROJECTION_PERSPECTIVE 1;

struct OffScreenData {
	Shader* screenShader;
	unsigned int texColorBuffer;
	unsigned int frameBuffer;
	unsigned int quadVAO, quadVBO;
	unsigned int rbo;
};

class Camera
{
private:
	static float quadVertices[];
	
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
	glm::mat4 vpMatrix;

	int projMode;

	OffScreenData offScreenData;

	float moveSpeed;
	float sensitivity;

	void InitOffScreenDraw();
	void ComputeMatrix();

public:
	Camera();
	~Camera();
	glm::mat4 VPmatrix();
	glm::mat4 Vmatrix();
	glm::mat4 Pmatrix();
	void Update();
	void EnableOffSreenBuffer();
	void PostDraw();

	Transform* transform;

};

