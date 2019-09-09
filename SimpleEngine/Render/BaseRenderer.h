#pragma once

#include <gl/glew.h>
#include <glm/glm.hpp>

#include <vector>
#include <string>

class BaseShader;
class Transform;
class Camera;
class MeshModel;
class BaseLight;
class Mesh;
class MeshModel;
class RenderData;

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

	void Initialize();

	bool castShadow;
	bool receiveShadow;//TODO not implemented

	virtual void Render(RenderData* renderData_) = 0;
	virtual void SetShader(BaseShader* shader_) {}
	virtual void SetShader() {}
	void SetMeshModel(MeshModel* meshModel_);

	MeshModel* GetMeshModel();

	void SetTransform(Transform* transform_);
	void ComputeMatrix(Camera* camera_);
	glm::mat4 Mmatrix();
	virtual void RenderShadowMap(BaseLight* light_) {}
};

