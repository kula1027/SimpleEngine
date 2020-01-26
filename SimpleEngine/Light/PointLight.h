#pragma once

#include "BaseLight.h"

class PointLight : public BaseLight
{
private:		
	glm::mat4 modelMatrix;//deferred에서 stencil test때 사용할 m matrix
	glm::mat4 matTranslation;
	glm::mat4 matScale;

	float range = 1;

protected:
	virtual void UpdateUboIntensity() override;
	void SetUbo_Range();

public:
	PointLight();
	~PointLight();

	void SetRange(float range_);
	float GetRange();

	glm::mat4 GetModelMatrix();

	virtual void UpdateUbo() override;
	virtual void OnTransformChanged() override;
	virtual void OnAttachedToObject(EngineObject* obj_) override;	
};

