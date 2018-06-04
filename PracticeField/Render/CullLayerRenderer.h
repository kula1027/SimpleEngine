#pragma once
#include "Renderer.h"

class CullLayerRenderer : public Renderer
{
public:
	CullLayerRenderer();
	~CullLayerRenderer();

	virtual void SetShader(Shader* shader_);
};

