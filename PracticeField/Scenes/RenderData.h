#pragma once

#include <vector>

class SkyBox;
class BaseRenderer;
class BaseLight;

class RenderData {
protected:
	

public:	

	RenderData();
	~RenderData();	

	std::vector<BaseRenderer*> renderers;
	std::vector<BaseLight*> lights;

	void AddRenderer(BaseRenderer* rdr_);
	void AddLight(BaseLight* light_);
};

