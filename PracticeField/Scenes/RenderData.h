#pragma once

#include <vector>

class SkyBox;
class Renderer;
class BaseLight;

class RenderData {
protected:
	

public:	

	RenderData();
	~RenderData();	

	std::vector<Renderer*> renderers;
	std::vector<BaseLight*> lights;

	void AddRenderer(Renderer* rdr_);
	void AddLight(BaseLight* light_);
};

