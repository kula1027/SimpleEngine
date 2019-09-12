#include "PerformanceCheck.h"
#include "../Bases/SP_Time.h"
#include "../Bases/InputModule.h"

#include <iostream>

unsigned int PerformanceCheck::numFrame = 0;
unsigned int PerformanceCheck::faceCount = 0;
unsigned int PerformanceCheck::drawCallCount = 0;
double PerformanceCheck::showingTimeItv = 3;
double PerformanceCheck::timeElapsed = 0;

void PerformanceCheck::OnEndFrame(){
	timeElapsed += SP_Time::deltaTime;
	numFrame++;

	if (timeElapsed >= showingTimeItv) {
		/*std::cout << "FPS: " << numFrame / showingTimeItv
			<< " / TPF: " << (timeElapsed / numFrame) * 1000 << " ms \n";*/
		timeElapsed -= showingTimeItv;
		numFrame = 0;
	}

	if (InputModule::IsPressed(GLFW_KEY_KP_0)) {
		std::cout << "Total Face Count: " << faceCount << std::endl;
		std::cout << "Total DrawCall Count: " << drawCallCount << std::endl;
	}
	faceCount = 0;
	drawCallCount = 0;
}

void PerformanceCheck::AddFaceCount(int fc){
	faceCount += fc;
}

void PerformanceCheck::AddDrawCallCount(int dc){
	drawCallCount += dc;
}

PerformanceCheck::~PerformanceCheck()
{
}
