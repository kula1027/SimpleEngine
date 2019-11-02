#pragma once

#include "BaseLight.h"

class SceneRenderData;

struct ShadowMapData {
	unsigned int resWidth = 2048;
	unsigned int resHeight = 2048;

	unsigned int depthMapTextureId;
	unsigned int depthMapFBO;
};

class DirectionalLight : public BaseLight{
private:
	BaseShader* shadowMapShader;

	unsigned int lightVPId;
	glm::mat4 lightView;	
	glm::mat4 lightProjection;	
	glm::mat4 lightVP;

	void InitShadowMap();	

	ShadowMapData shadowMapData;


protected:
	virtual void SetUbo_Intensity() override;

public:
	DirectionalLight();
	virtual ~DirectionalLight();

	void RenderShadowMap(SceneRenderData* srd_);

	float near_plane = 0.1f, far_plane = 20.0f;

	ShadowMapData GetShadowMapData();
	void BindShadowMap();

	virtual void SetUbo() override;
	virtual void OnTransformChanged() override;	
	virtual void OnAttachedToObject(EngineObject* obj_) override;
};