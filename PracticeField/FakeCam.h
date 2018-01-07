#pragma once
#include "IUpdatable.h"

class FakeCam : public IUpdatable
{
public:
	FakeCam();
	~FakeCam();

	virtual void OnUpdate();

	float moveSpeed = 20;
};

