#pragma once

#include "MeshRenderer.h"

class Renderer_Forward : public MeshRenderer {
private:
	BaseShader* shader;

public:
	Renderer_Forward();
	virtual ~Renderer_Forward();

	virtual void RenderMesh() override;
};

