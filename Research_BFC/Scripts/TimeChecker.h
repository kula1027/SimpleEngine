#include <Bases/BasesBundle.h>

#pragma once
class TimeChecker : public BaseScript {
private:
	float showingTimeItv = 5;
	float timePrev;
	float timeAcc;
	int frameCount;

public:
	TimeChecker();
	~TimeChecker();

	virtual void OnUpdate();
	virtual void OnStart();
};

