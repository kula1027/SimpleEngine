#include "TimeChecker.h"

#include "Time.h"
#include <iostream>

TimeChecker::TimeChecker(){
}


TimeChecker::~TimeChecker(){
}

void TimeChecker::OnUpdate(){	
	timeAcc += Time::deltaTime;
	frameCount++;

	if (timeAcc >= 1) {
		std::cout << "FPS: " << frameCount
			<< " / TPF: " << timeAcc / frameCount << "\n";
		timeAcc -= 1;
		frameCount = 0;
	}
}

void TimeChecker::OnStart(){
	timeAcc = 0;
	frameCount = 0;	
}
