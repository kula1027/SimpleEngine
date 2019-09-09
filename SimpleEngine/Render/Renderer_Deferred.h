#pragma once

#include "BaseRenderer.h"

class Renderer_Deferred {
public:
	Renderer_Deferred();
	~Renderer_Deferred();

	virtual void Render(RenderData* renderData_);

};

