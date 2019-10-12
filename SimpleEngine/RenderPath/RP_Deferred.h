#pragma once
#include "RenderPath.h"

class Mesh;

class RP_Deferred : public RenderPath{
private:
	BaseShader* shaderGeometry;
	BaseShader* shaderDirectional;
	BaseShader* shaderPointStencilPass;
	BaseShader* shaderPointLightPass;
	Mesh* meshSphere;

	void SetupFrameBuffers();
	void SetupShaders();
	void GeometryPass(SceneRenderData* sceneRenderData_);
	void LightPass();
	void LightPass_AmbientDirectional();
	void LightPass_Point();
	void AdditionalForwardPass(SceneRenderData* sceneRenderData_);

public:
	unsigned int gBuffer;
	unsigned int gPosition, gNormal, gAlbedoSpec;
	unsigned int attachments[3];

	RP_Deferred();
	virtual ~RP_Deferred();

	virtual void Render(SceneRenderData* renderData);

};