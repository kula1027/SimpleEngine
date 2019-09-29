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
class ShaderForward;

class MeshRenderer : public BaseComponent{
protected:
	/*std::string defaultVS;
	std::string defaultFS;
	BaseShader* shader = NULL;*/	
	//std::vector<BaseRenderMaterial*> renderMaterials;
	//for Forward Rendering
	ShaderForward* shaderForward;

	Transform* transform = NULL;//ref to engineObject transform
	MeshModel* meshModel = NULL;

	glm::mat4 modelMatrix;
	glm::mat4 mvpMatrix;

	virtual void SetAdditionalShaderData(BaseShader* shader_) {}
	virtual void SetUniformAdditional() {}

public:
	MeshRenderer();
	MeshRenderer(MeshModel* meshModel_);
	virtual ~MeshRenderer();

	void Initialize(MeshModel* meshModel_);

	bool castShadow;
	bool receiveShadow;//TODO not implemented

	virtual void RenderMesh_Forward(Camera* camera_, std::vector<BaseLight*>* lights_);
	virtual void RenderMesh();

	void SetMeshModel(MeshModel* meshModel_);

	MeshModel* GetMeshModel();
	BaseShader* GetShader();

	void ComputeMatrix(Camera* camera_);
	glm::mat4 Mmatrix();
	glm::mat4 MVPmatrix();
	virtual void RenderShadowMap(BaseLight* light_) {}
	virtual void OnAttachedToObject(EngineObject*) override;	
};

