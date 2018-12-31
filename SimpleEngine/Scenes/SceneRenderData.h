#pragma once

#include <vector>

class SkyBox;
class BaseRenderer;
class BaseLight;

// Scene -> RenderPath Param Obj
class SceneRenderData {
protected:
	

public:	
	SceneRenderData();
	~SceneRenderData();	

	std::vector<BaseRenderer*> renderers;
	std::vector<BaseLight*> lights;

	void AddRenderer(BaseRenderer* rdr_);
	void AddLight(BaseLight* light_);
};

