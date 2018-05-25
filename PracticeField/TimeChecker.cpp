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

	if (timeAcc >= showingTimeItv) {
		std::cout << "FPS: " << frameCount / showingTimeItv << std::endl;
			//<< " / TPF: " << 1.0f / (frameCount / showingTimeItv) << "\n";
		timeAcc -= showingTimeItv;
		frameCount = 0;
	}
}

void TimeChecker::OnStart(){
	timeAcc = 0;
	frameCount = 0;	
}
