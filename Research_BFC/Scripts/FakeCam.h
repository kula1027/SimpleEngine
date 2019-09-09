#pragma once

#include <Bases/BasesBundle.h>

class FakeCam : public BaseScript
{
public:
	FakeCam();
	~FakeCam();

	virtual void OnUpdate();

	float moveSpeed = 30;
};

