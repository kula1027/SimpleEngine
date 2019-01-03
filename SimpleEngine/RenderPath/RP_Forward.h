#pragma once
#include "RenderPath.h"

class RP_Forward : public RenderPath{
private:

protected:
	

	
	void EnableOffSreenBuffer(Camera* cam_);
	void PostDraw();

public:
	RP_Forward();
	~RP_Forward();

	virtual void Render(Camera* mainCamera_, SceneRenderData* renderData_);	
	virtual void Initialize();
};

