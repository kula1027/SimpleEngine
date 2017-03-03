#pragma once

#include <GLFW\glfw3.h>

class GameWindow
{
private:
	GLFWwindow* window;
	int width;
	int height;

public:
	GameWindow(int w_, int h_, const char* name_);
	int GetWidth();
	int GetHeight();
	GLFWwindow* GetWindow();
	bool ShouldClose();
	~GameWindow();
};

