#pragma once

#include <vector>

class SkyBox;
class Renderer;
class BaseLight;

class RenderData
{
public:	

	RenderData();
	~RenderData();

	SkyBox * skybox;
	std::vector<Renderer*> renderers;
	std::vector<BaseLight*> lights;
};

