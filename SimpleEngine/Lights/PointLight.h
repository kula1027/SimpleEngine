#pragma once

#include "BaseLight.h"

class PointLight : public BaseLight
{
private:		
	glm::mat4 modelMatrix;//deferred에서 stencil test때 사용할 m matrix

public:
	PointLight();
	~PointLight();

	float range;	

	glm::mat4 GetModelMatrix();
	virtual void OnTransformChanged() override;
};

