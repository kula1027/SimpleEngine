#include "Time.h"

#include <GLFW\glfw3.h>
#include <iostream>


double Time::prevTime = 0;
float Time::deltaTime = 0;
double Time::accTime = 0;

double Time::GetCurrentTime(){
	return glfwGetTime();
}

void Time::Tick() {
	double currentTime = glfwGetTime();
	deltaTime = currentTime - prevTime;
	prevTime = currentTime;
}

void Time::PrintRenderTime(double t){
	static int frameCount;

	if (accTime > t) {
		if (frameCount != 0)
			std::cout << "RenderTime: " << accTime / frameCount << std::endl
					  << "FramePerSec: " << frameCount / t << std::endl;

		frameCount = 0;
		accTime -= t;
	}
	frameCount++;
	accTime += deltaTime;
}

