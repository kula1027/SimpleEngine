#pragma once

#include <Bases/BaseComponent.h>

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

class MeshRenderer : public BaseComponent{
protected:	
	Transform* transform = NULL;//ref to engineObject transform
	MeshModel* meshModel = NULL;

	glm::mat4 modelMatrix;
	glm::mat4 mvpMatrix;	
	
	bool castShadow;
	bool receiveShadow;

public:
	MeshRenderer();
	MeshRenderer(MeshModel* meshModel_);
	virtual ~MeshRenderer();

	virtual void RenderMesh();
	virtual void RenderMesh_NoTexture();

	void SetMeshModel(MeshModel* meshModel_);

	MeshModel* GetMeshModel();
	BaseShader* GetShader();

	void ComputeMatrices(Camera* camera_);
	glm::mat4 Mmatrix();
	glm::mat4 MVPmatrix();
	virtual void RenderShadowMap(BaseLight* light_) {}
	virtual void OnAttachedToObject(EngineObject*) override;		

	int renderOrder;
};

