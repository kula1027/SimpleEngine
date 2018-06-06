#pragma once

#include "../Bases/BasesBundle.h"

class FakeCam : public IUpdatable
{
public:
	FakeCam();
	~FakeCam();

	virtual void OnUpdate();

	float moveSpeed = 10;
};

