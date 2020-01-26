#pragma once

#include <vector>
#include <Render/Renderer/MeshRenderer.h>

class Transform;
class RenderData;
class EngineObject;

class Renderer_Instanced : public MeshRenderer
{
private:
	BaseShader* shader;		

public:
	Renderer_Instanced();
	~Renderer_Instanced();	

	void InitInstanced();
	virtual void RenderMesh_NoTexture() override;
	virtual void RenderMesh() override;	
};

