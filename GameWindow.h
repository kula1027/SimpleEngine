#pragma once

#include <GLFW\glfw3.h>
#include <iostream>

class GameWindow
{
private:
	static GLFWwindow* window;
	static int width;
	static int height;
	static bool isFocused;

	static void FocusCallBack(GLFWwindow* window_, int focused);

public:
	static void Init(int w_, int h_, const char* name_);
	static int GetWidth();
	static int GetHeight();
	static GLFWwindow* GetWindow();
	static bool IsFocused();
	static bool ShouldClose();
};

