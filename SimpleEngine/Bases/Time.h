#pragma once

class Time
{
private:
	static double prevTime;
	static double accTime;

public:
	static float deltaTime;
	static double GetCurrentTime();
	static void Tick();
	static void PrintRenderTime(double t);
};

