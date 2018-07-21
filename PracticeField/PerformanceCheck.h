#pragma once
class PerformanceCheck
{
public:
	static void OnEndFrame();
	static void AddFaceCount(int);
	static void AddDrawCallCount(int);

private:
	PerformanceCheck() {}
	~PerformanceCheck();

	static unsigned int numFrame;
	static unsigned int faceCount;
	static unsigned int drawCallCount;
	static double showingTimeItv;
	static double timeElapsed;
};

