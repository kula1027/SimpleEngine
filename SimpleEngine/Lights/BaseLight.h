#pragma once

#include <glm\glm.hpp>
#include <string>
#include <Bases/BaseComponent.h>

class BaseShader;

enum LightType {
	LightType_Directional = 0,
	LightType_Point = 1,
	LightType_Spot = 2
};

class BaseLight : public BaseComponent{
private:
	float intensity = 1;

protected:
	LightType lightType;
	int startAddrUbo;

	glm::vec3 color;
	virtual void SetUbo_Intensity() {}

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

	bool castShadow;
	
	virtual void InitShadowMap() {}
	virtual void EnableShadowMapBuffer() {}

	virtual void OnAttachedToObject(EngineObject* obj_) override;	
	virtual void SetUbo() {}
};

