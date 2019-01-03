#pragma once

#include "DefaultRenderer.h"
#include <vector>

#define DefaultVS_Ist "instanced.vert"

class Transform;
class GameObject;

class InstancedRenderer : public DefaultRenderer
{
private:
	glm::mat4* matriceModel;	

	glm::mat4 vpMatrix;

	int childTransformCount = 0;

public:
	InstancedRenderer();
	~InstancedRenderer();

	virtual void SetShader(BaseShader* shader_);
	virtual void Render(RenderData* renderData_);
	virtual void SetMeshModel(MeshModel* meshModel_);

	void InitInstanced();
};
