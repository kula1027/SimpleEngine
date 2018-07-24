#pragma once
#include "RenderPipeLine.h"

class RPL_Forward : public RenderPipeLine{
public:
	RPL_Forward();
	~RPL_Forward();

	virtual void Render(RenderData* renderData);
};

