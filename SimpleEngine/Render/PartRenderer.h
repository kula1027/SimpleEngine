#pragma once
#include "DefaultRenderer.h"

class PartRenderer : public DefaultRenderer
{
private:
	int bindedBuffer = -1;
public:
	virtual void Render(Camera* cam, std::vector<BaseLight*> lights);
};