#pragma once

#include <GLFW\glfw3.h>

class Time
{
private:
	static double prevTime;

public:
	static float deltaTime;
	static void Tick();

};

