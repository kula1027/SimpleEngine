#pragma once
#include "SceneRenderData.h"

class Renderer_Deferred;

class SceneRenderData_df : public SceneRenderData {
public:
	SceneRenderData_df();
	~SceneRenderData_df();

	std::vector<Renderer_Deferred*> renderers;
};

