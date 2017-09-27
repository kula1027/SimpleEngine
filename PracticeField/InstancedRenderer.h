#pragma once

#include "Renderer.h"
#include <vector>

#define DefaultVS_Ist "instanced.vert"

class Transform;
class GameObject;

class InstancedRenderer : public Renderer
{
private:
	std::vector<Transform*> attachedTransforms;
	glm::mat4* matriceModel;

	void InitInstanced(Mesh* currentMesh_);

public:
	InstancedRenderer();
	~InstancedRenderer();

	void AddObject(GameObject* go_);
	void AddTransform(Transform* tr_);

	virtual void SetDefaultShader();
	virtual void Render(Camera* cam, std::vector<BaseLight*> lights);
	virtual void SetMeshModel(MeshModel* meshModel_);
};

