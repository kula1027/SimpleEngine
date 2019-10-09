#pragma once
#include "RenderPath.h"

class ShaderDeferredGeo;
class ShaderDeferredLight;

class RP_Deferred : public RenderPath{
private:
	ShaderDeferredGeo* shaderDeferredGeo;
	BaseShader* shaderDeferredAmbient;
	BaseShader* shaderDeferredDirectional;
	BaseShader* shaderDeferredPoint;

	void SetupFrameBuffers();
	void SetupShaders();
	void ProcessLightPass();

public:
	unsigned int gBuffer;
	unsigned int gPosition, gNormal, gAlbedoSpec;
	unsigned int attachments[3];

	RP_Deferred();
	~RP_Deferred();

	virtual void Render(SceneRenderData* renderData);

};