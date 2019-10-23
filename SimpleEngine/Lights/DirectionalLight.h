#pragma once

#include "BaseLight.h"

struct ShadowMapData {
	unsigned int resWidth = 2048;
	unsigned int resHeight = 2048;

	unsigned int depthMapTextureId;
	unsigned int depthMapFBO;
};

class DirectionalLight : public BaseLight{
private:
	BaseShader* shadowMapShader;

	unsigned int lightSpaceMatrixId;
	glm::mat4 lightView;	
	glm::mat4 lightProjection;
	void InitShadowMap();	

	ShadowMapData shadowMapData;


protected:
	virtual void SetUbo_Intensity() override;

public:
	DirectionalLight();
	virtual ~DirectionalLight();

	void EnableShadowMapBuffer();

	float near_plane = 0.1f, far_plane = 1000.0f;

	ShadowMapData GetShadowMapData();

	virtual void SetUbo() override;
	virtual void OnTransformChanged() override;	
};