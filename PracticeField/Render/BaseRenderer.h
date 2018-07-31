#pragma once

#include <gl/glew.h>
#include <glm\glm.hpp>
#include <vector>
#include <string>

class BaseShader;
class Transform;
class Camera;
class MeshModel;
class BaseLight;
class Mesh;
class MeshModel;

struct ID_matrice {
	GLuint mvp = -1;
	GLuint view = -1;
	GLuint model = -1;
	GLuint vp = -1;
};

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

class BaseRenderer {
protected:
	std::string defaultVS;
	std::string defaultFS;
	BaseShader* shader = NULL;

	Transform* transform = NULL;//ref to gameObject transform
	MeshModel* meshModel = NULL;

	glm::mat4 modelMatrix;
	glm::mat4 mvpMatrix;

public:
	BaseRenderer();
	BaseRenderer(MeshModel* meshModel_);
	BaseRenderer(Transform* transform_);
	~BaseRenderer();

	bool castShadow;
	bool cullingEnabled;
	bool lineDrawEnabled;

	virtual void Render(Camera* cam, std::vector<BaseLight*> lights) = 0;
	virtual void SetShader(BaseShader* shader_) {}
	virtual void SetShader() {}
	void SetMeshModel(MeshModel* meshModel_);

	MeshModel* GetMeshModel();

	void SetTransform(Transform* transform_);
	void ComputeMatrix();
	virtual void RenderShadowMap(BaseLight* light_) {}
};

