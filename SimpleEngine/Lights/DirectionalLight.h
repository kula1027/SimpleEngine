#pragma once

#include "BaseLight.h"

class SceneRenderData;
class Camera;

struct ShadowMapData {
	unsigned int resWidth = 2048 * 2;
	unsigned int resHeight = 2048 * 2;

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
	float shadow_near = 0.1f, shadow_far = 200;
	float shadow_size = 30;

	void UpdateUboDirection();
	void UpdateUboVP();

protected:
	virtual void UpdateUboIntensity() override;

public:
	DirectionalLight();
	virtual ~DirectionalLight();

	void RenderShadowMap(SceneRenderData* srd_, Camera* camera_);	

	ShadowMapData GetShadowMapData();
	void BindShadowMap();

	virtual void UpdateUbo() override;
	virtual void UpdateUboColor() override;
	virtual void OnTransformChanged() override;	
	virtual void OnAttachedToObject(EngineObject* obj_) override;
};