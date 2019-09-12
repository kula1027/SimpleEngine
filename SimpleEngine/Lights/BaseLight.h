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
	LightType_Directional,
	LightType_Point,
	LightType_Cone
};

class BaseLight : public BaseComponent{
public:
	BaseLight();
	~BaseLight();

	void SetIntensity(float intensity_);
	float GetIntensity();
	void SetColor(glm::vec3 color_);
	glm::vec3 GetColor();
	void SetPosition(glm::vec3 position_);
	glm::vec3 GetPosition();		

	unsigned int modelMatrixId;
	glm::mat4 lightSpaceMatrix;

	ShadowData shadowData;	

	bool isShadowCaster;
	
	virtual void InitShadowMap() {}
	virtual void EnableShadowMapBuffer() {}

	virtual void OnAttachedToObject(EngineObject* obj_) override;

protected:
	LightType lightType;
	bool uniformValueHasChanged = true;

	glm::vec3 color;
	glm::vec3 position;
	float intensity;

	bool isStatic = false;
};

