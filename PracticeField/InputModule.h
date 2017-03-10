#pragma once
#include <iostream>
#include <GLFW\glfw3.h>
#include "GameWindow.h"

class InputModule
{
private:
	static double mousePos[2];
public:
	static void Init();
	static void CheckInput();
	static bool IsPressed(int key);
	static double* GetMousePos();
};