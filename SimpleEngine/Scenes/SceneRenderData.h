#pragma once

#include <vector>

class SkyBox;
class MeshRenderer;
class BaseLight;
class BaseRenderMaterial;

// Scene -> RenderPath Param Obj
class SceneRenderData {
protected:
	

public:	
	SceneRenderData();
	~SceneRenderData();

	std::vector<MeshRenderer*> renderers;
	std::vector<BaseLight*> lights;

	void AddRenderer(MeshRenderer* rdr_);
	void AddLight(BaseLight* light_);
};

