#include "IUpdatable.h"

#pragma once
class TimeChecker : public IUpdatable {
private:
	float showingTimeItv = 2;
	float timePrev;
	float timeAcc;
	int frameCount;

public:
	TimeChecker();
	~TimeChecker();

	virtual void OnUpdate();
	virtual void OnStart();
};

