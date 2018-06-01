#pragma once

#include "BaseLight.h"

class PointLight : public BaseLight
{
private:		

public:
	PointLight();
	~PointLight();

	float range;

	void Update();
};

