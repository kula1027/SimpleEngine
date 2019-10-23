#pragma once

#include <vector>


class MeshRenderer;
class BaseLight;
class BaseRenderMaterial;
class LightManager;

// Scene -> RenderPath Param Obj
class SceneRenderData {
protected:
	

public:	
	SceneRenderData();
	~SceneRenderData();

	std::vector<MeshRenderer*> renderQueue_Deferred;
	std::vector<MeshRenderer*> renderQueue_Forward;	
	

	void AddRenderer(MeshRenderer* rdr_);	
};

