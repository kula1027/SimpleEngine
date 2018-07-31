#pragma once
#include "RenderPath.h"

class RP_Forward : public RenderPath{
private:

protected:
	OffScreenData offScreenData;

	void InitOffScreenDraw();
	void EnableOffSreenBuffer(Camera* cam_);
	void PostDraw();

public:
	RP_Forward();
	~RP_Forward();

	virtual void Render(Camera* mainCamera_, RenderData* renderData_);	
	virtual void Initialize();
};

