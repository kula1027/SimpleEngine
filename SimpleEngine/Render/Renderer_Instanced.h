#pragma once

#include <vector>
#include <Render/MeshRenderer.h>

class Transform;
class EngineObject;

class Renderer_Instanced : public MeshRenderer
{
private:
	BaseShader* shader;		

public:
	Renderer_Instanced();
	~Renderer_Instanced();	

	void InitInstanced();
	virtual void RenderMesh() override;
};

