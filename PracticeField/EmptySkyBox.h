#pragma once
#include "SkyBox.h"

class EmptySkyBox : public SkyBox
{
public:
	EmptySkyBox();
	~EmptySkyBox();
	virtual void Render(Camera* cam);
};

