#pragma once
#include "RenderPipeLine.h"

class RPL_Forward : public RenderPipeLine{
private:

protected:
	OffScreenData offScreenData;

	void InitOffScreenDraw();
	void EnableOffSreenBuffer(Camera* cam_);
	void PostDraw();

public:
	RPL_Forward();
	~RPL_Forward();

	virtual void Render(Camera* mainCamera_, RenderData* renderData_);	
	
};

