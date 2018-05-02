#include "InputModule.h"

#include "GameWindow.h"
#include <iostream>

double InputModule::mousePos[] = { 0, 0 };

void InputModule::Init() {
	glfwSetInputMode(GameWindow::GetWindow(), GLFW_STICKY_KEYS, GL_TRUE);	
	glfwSetInputMode(GameWindow::GetWindow(), GLFW_CURSOR, GLFW_CURSOR_NORMAL);

	glfwSetCursorPos(GameWindow::GetWindow(), GameWindow::GetWidth() / 2, GameWindow::GetHeight() / 2);
}

void InputModule::CheckInput(){
	glfwGetCursorPos(GameWindow::GetWindow(), &mousePos[0], &mousePos[1]);
	glfwSetCursorPos(GameWindow::GetWindow(), GameWindow::GetWidth() / 2, GameWindow::GetHeight() / 2);
}

bool InputModule::IsPressed(int key){
	return glfwGetKey(GameWindow::GetWindow(), key) == GLFW_PRESS;
}

double * InputModule::GetMousePos(){
	return mousePos;
}
