#pragma once
#include "Renderer.h"

class PartRenderer : public Renderer
{
private:
	int bindedBuffer = -1;
public:
	virtual void Render(Camera* cam, std::vector<BaseLight*> lights);
};