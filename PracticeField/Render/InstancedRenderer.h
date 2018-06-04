#pragma once

#include "Renderer.h"
#include <vector>

#define DefaultVS_Ist "instanced.vert"

class Transform;
class GameObject;

class InstancedRenderer : public Renderer
{
private:
	glm::mat4* matriceModel;	

	glm::mat4 vpMatrix;

	int childTransformCount = 0;

public:
	InstancedRenderer();
	~InstancedRenderer();

	virtual void SetShader(Shader* shader_);
	virtual void SetDefaultShader();
	virtual void Render(Camera* cam_, std::vector<BaseLight*> lights_);
	virtual void SetMeshModel(MeshModel* meshModel_);

	void InitInstanced();
};

