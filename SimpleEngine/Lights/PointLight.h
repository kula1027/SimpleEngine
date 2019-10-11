#pragma once

#include "BaseLight.h"

class PointLight : public BaseLight
{
private:		
	glm::mat4 modelMatrix;//deferred���� stencil test�� ����� m matrix

public:
	PointLight();
	~PointLight();

	float range;	

	glm::mat4 GetModelMatrix();
	virtual void OnTransformChanged() override;
};

