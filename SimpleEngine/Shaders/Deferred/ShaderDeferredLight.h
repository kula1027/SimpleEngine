#pragma once
#include <Shaders/BaseShader.h>

class ShaderDeferredLight : public BaseShader {
public:
	ShaderDeferredLight();
	~ShaderDeferredLight();

	void SetLightUniforms(BaseLight* light_, int lightIdx_);
};

