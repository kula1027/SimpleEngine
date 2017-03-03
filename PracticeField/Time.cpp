#include "Time.h"


double Time::prevTime = 0;
float Time::deltaTime = 0;

void Time::Tick() {
	double currentTime = glfwGetTime();
	deltaTime = currentTime - prevTime;
	prevTime = currentTime;
}