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
	virtual void UpdateUboIntensity() {}	
	virtual void UpdateUboColor() {}

public:
	BaseLight();
	~BaseLight();

	void SetIntensity(float intensity_);
	float GetIntensity();
	void SetColor(glm::vec3 color_);
	glm::vec3 GetColor();		
	LightType GetLightType();
	void SetStartAddrUbo(int startAddr_);	
	

	bool castShadow;
	
	virtual void InitShadowMap() {}
	virtual void RenderShadowMap() {}

	virtual void OnAttachedToObject(EngineObject* obj_) override;	
	virtual void UpdateUbo() {}
};

