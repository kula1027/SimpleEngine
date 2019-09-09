#include "SP_Time.h"

#include <GLFW\glfw3.h>
#include <iostream>


double SP_Time::prevTime = 0;
float SP_Time::deltaTime = 0;
double SP_Time::accTime = 0;

double SP_Time::GetCurrentTime(){
	return glfwGetTime();
}

void SP_Time::Tick() {
	double currentTime = glfwGetTime();
	deltaTime = currentTime - prevTime;
	prevTime = currentTime;
}

void SP_Time::PrintRenderTime(double t){
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

