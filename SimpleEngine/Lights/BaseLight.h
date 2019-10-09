#pragma once

#include <glm\glm.hpp>
#include <string>
#include <Bases/BaseComponent.h>

class BaseShader;

struct ShadowData {
	unsigned int resWidth = 2048;
	unsigned int resHeight = 2048;
		
	unsigned int depthMapTextureId;
	unsigned int depthMapFBO;
};
enum LightType {
	LightType_Directional = 0,
	LightType_Point = 1,
	LightType_Spot = 2
};

class BaseLight : public BaseComponent{
public:
	BaseLight();
	~BaseLight();

	void SetIntensity(float intensity_);
	float GetIntensity();
	void SetColor(glm::vec3 color_);
	glm::vec3 GetColor();		
	LightType GetLightType();
	void SetStartAddrUbo(int startAddr_);


	unsigned int modelMatrixId;
	glm::mat4 lightSpaceMatrix;

	ShadowData shadowData;	

	bool isShadowCaster;
	
	virtual void InitShadowMap() {}
	virtual void EnableShadowMapBuffer() {}

	virtual void OnAttachedToObject(EngineObject* obj_) override;
	virtual void SetUniformsUbo() {}

protected:
	LightType lightType;
	int startAddrUbo;	

	glm::vec3 color;	
	float intensity;

	bool isStatic = false;
};

