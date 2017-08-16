#pragma once

#include <gl\glew.h>
#include <glm\glm.hpp>
#include <vector>

struct ID_dLight {
	GLuint direction;
	GLuint color;
	GLuint power;
};

struct ID_pLight {
	GLuint position;
	GLuint color;
	GLuint power;
};

class Shader;
class Transform;
class Camera;
class MeshModel;
class BaseLight;

class Renderer{
private:
	Shader* shader;
	GLuint mvpMatrixID;
	GLuint viewMatrixID;
	GLuint modelMatrixID;

	GLuint difTexCountID;

	ID_dLight id_dLight;
	ID_pLight id_pLight;

	glm::mat4 modelMatrix;
	glm::mat4 mvpMatrix;

	Transform* transform;//ref to gameObject transform

public:
	Renderer(Transform* transform_);
	~Renderer();

	void SetShader(Shader* shader_);
	void Render(Camera* cam, std::vector<BaseLight*> lights, MeshModel* meshModel);
	void ComputeModelMatrix(Camera* cam);
};

