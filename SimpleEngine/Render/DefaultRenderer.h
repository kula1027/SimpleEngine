#pragma once

#include "MeshRenderer.h"

#define AttrLoc_IstMatrix0 3
#define AttrLoc_IstMatrix1 4
#define AttrLoc_IstMatrix2 5
#define AttrLoc_IstMatrix3 6

class DefaultRenderer : public MeshRenderer{
protected:			

	virtual void SetAdditionalShaderData(BaseShader* shader_) {}
	virtual void SetUniformAdditional() {}

public:	
	DefaultRenderer();
	~DefaultRenderer();

	virtual void RenderShadowMap(BaseLight* light_) override;

	virtual void SetShader(BaseShader* shader_) override;
	virtual void SetShader() override;


};

