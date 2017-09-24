#pragma once

#include <gl\glew.h>
#include <glm\glm.hpp>
#include <vector>

struct ID_dLight {
	GLuint direction;
	GLuint color;
	GLuint power;
	GLuint lightSpaceMatrix;
	GLuint shadowMap;
};

struct ID_pLight {
	GLuint position;
	GLuint color;
	GLuint power;
};

struct ID_diffuseTexture {
	GLuint count;
	GLuint id;
};

struct ID_specularTexture {
	GLuint count;
	GLuint id;
};

struct Outline {
	bool draw = false;
	float thickness = 0.08f;
	int id_thickness;
	int id_color;
	glm::vec3 color = glm::vec3(0.2, 0.5, 1.0);

};

class Shader;
class Transform;
class Camera;
class MeshModel;
class BaseLight;
class Mesh;
class MeshModel;

class Renderer{
protected:
	Shader* shader;
	Shader* outlineShader;
	GLuint mvpMatrixID;
	GLuint viewMatrixID;
	GLuint modelMatrixID;

	ID_dLight id_dLight;
	ID_pLight id_pLight;
	ID_diffuseTexture id_diffuse;
	ID_specularTexture id_specular;

	glm::mat4 modelMatrix;
	glm::mat4 mvpMatrix;

	Transform* transform;//ref to gameObject transform
	MeshModel* meshModel;

	void ApplyTexture(Mesh* processingMesh_);

public:	
	Renderer();
	Renderer(Transform* transform_);
	~Renderer();

	void SetTransform(Transform* transform_);
	void SetShader(Shader* shader_);
	void SetMeshModel(MeshModel* meshModel_);
	virtual void Render(Camera* cam, std::vector<BaseLight*> lights);
	void RenderShadowMap(BaseLight* light_);
	void RenderScaledUp();
	void ComputeModelMatrix(Camera* cam);

	Outline outline;
	bool castShadow;
	bool cullingEnabled;
};

