#pragma once
#include <Render/DefaultRenderer.h>

#define VS_LayerCull "3layer.vert" //·»´õ·¯ Àü¿ë ½¦ÀÌ´õ
#define FS_LayerCulled "3layer.frag"


class CullLayerRenderer : public DefaultRenderer
{
private:
	int id_refViewPos;

protected:
	void SetAdditionalShaderData(BaseShader* shader_);
	void SetUniformAdditional();	

public:
	CullLayerRenderer();
	~CullLayerRenderer();

	void SetShader();	
	

	Transform* refTransform;
};

