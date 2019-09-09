#pragma once
#include "RenderPath.h"

class GBufferShader;

class RP_Deferred : public RenderPath{
private:
	GBufferShader* gBufferShader;

public:
	unsigned int gBuffer;
	unsigned int gPosition, gNormal, gAlbedoSpec;
	unsigned int attachments[3];



	RP_Deferred();
	~RP_Deferred();

	virtual void Initialize();
	virtual void Render(Camera* mainCamera, SceneRenderData* renderData);
};