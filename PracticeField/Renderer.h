#pragma once
#include <glm\glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <vector>

#include "Light.h"
#include "MeshModel.h"
#include "Transform.h"
#include "Camera.h"


class Renderer{
private:
	Shader* shader;
	GLuint mvpMatrixID;
	GLuint viewMatrixID;
	GLuint modelMatrixID;
	GLuint lightPosID;
	GLuint lightColorID;
	GLuint lightPowerID;

	glm::mat4 modelMatrix;
	glm::mat4 mvpMatrix;

	Transform* transform;//ref to gameObject transform

public:
	Renderer(Transform* transform_);
	~Renderer();

	void SetShader(Shader* shader_);
	void Render(Camera* cam, std::vector<Light*> lights, MeshModel* meshModel);
	void ComputeModelMatrix(Camera* cam);
};

