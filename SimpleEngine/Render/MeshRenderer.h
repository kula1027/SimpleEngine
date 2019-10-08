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


//default RenderMode = deferred
class MeshRenderer : public BaseComponent{
protected:
	/*std::string defaultVS;
	std::string defaultFS;
	BaseShader* shader = NULL;*/	
	//std::vector<BaseRenderMaterial*> renderMaterials;
	//for Forward Rendering
	BaseShader* shaderForward;

	Transform* transform = NULL;//ref to engineObject transform
	MeshModel* meshModel = NULL;

	glm::mat4 modelMatrix;
	glm::mat4 mvpMatrix;
	
	virtual void SetUniformAdditional() {}

	bool isRenderModeForward;
	bool castShadow;
	bool receiveShadow;

public:
	MeshRenderer();
	MeshRenderer(MeshModel* meshModel_);
	virtual ~MeshRenderer();

	void SetRenderModeForward(bool isForward_);
	void SetRenderModeForward(BaseShader* sf_);
	bool GetRenderModeForward();

	virtual void RenderMesh_Forward(Camera* camera_);
	virtual void RenderMesh();

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

