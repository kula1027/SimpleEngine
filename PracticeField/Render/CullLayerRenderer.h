#pragma once
#include "Renderer.h"

#define VS_LayerCull "3layer.vert"
#define FS_LayerCulled "3layer.frag"


class CullLayerRenderer : public Renderer
{
private:
	int id_refViewPos;

protected:
	void SetAdditionalShaderData(Shader* shader_);
	void SetUniformAdditional();	

public:
	CullLayerRenderer();
	~CullLayerRenderer();

	void SetShader();	
	

	Transform* refTransform;
};

