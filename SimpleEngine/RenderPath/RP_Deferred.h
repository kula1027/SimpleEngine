#pragma once
#include "RenderPath.h"

class RP_Deferred : RenderPath{
public:
	RP_Deferred();
	~RP_Deferred();

	virtual void Initialize();
	virtual void Render(Camera* mainCamera, SceneRenderData* renderData);
};

