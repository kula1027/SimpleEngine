#pragma once
#include "Renderer.h"

class PartRenderer : public Renderer
{
public:
	virtual void Render(Camera* cam, std::vector<BaseLight*> lights);
};

