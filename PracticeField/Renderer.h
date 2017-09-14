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
class Mesh;

class Renderer{
protected:
	Shader* shader;
	GLuint mvpMatrixID;
	GLuint viewMatrixID;
	GLuint modelMatrixID;

	GLuint difTexCountID;
	GLuint specTexCountID;

	ID_dLight id_dLight;
	ID_pLight id_pLight;

	glm::mat4 modelMatrix;
	glm::mat4 mvpMatrix;

	Transform* transform;//ref to gameObject transform
	MeshModel* meshModel;//ref to gameObject meshModel

	void ApplyTexture(Mesh* processingMesh_);

public:	
	Renderer();
	Renderer(Transform* transform_);
	~Renderer();

	void SetReferences(Transform* transform_, MeshModel* meshModel_);
	void SetShader(Shader* shader_);
	virtual void Render(Camera* cam, std::vector<BaseLight*> lights);
	void ComputeModelMatrix(Camera* cam);
};

