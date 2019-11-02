#pragma once
#include "RenderPath.h"

class Mesh;

class RP_Deferred : public RenderPath{
private:
	BaseShader* shaderGeometry;
	BaseShader* shaderDirectional;
	BaseShader* shaderPointStencilPass;
	BaseShader* shaderPointLightPass;
	BaseShader* shaderShadowDepth;
	Mesh* meshSphere;

	unsigned int gBuffer;
	unsigned int gPosition, gNormal, gAlbedoSpec;
	unsigned int attachments[3];

	void SetupFrameBuffers();
	void SetupShaders();

	void ComputeMatrices();
	void GeometryPass();
	void RenderShadowMap();
	void LightPass();
	void LightPass_AmbientDirectional();
	void LightPass_Point();	void AdditionalForwardPass();


public:


	RP_Deferred();
	virtual ~RP_Deferred();

	virtual void Render(SceneRenderData* renderData);	
};