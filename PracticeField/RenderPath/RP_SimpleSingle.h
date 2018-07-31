#pragma once
#include "RenderPath.h"

class RP_SimpleSingle :	public RenderPath {
public:
	RP_SimpleSingle();
	~RP_SimpleSingle();

	virtual void Render(Camera* mainCamera, RenderData* renderData);
};

