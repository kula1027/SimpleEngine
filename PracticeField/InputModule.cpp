#include "InputModule.h"


GameWindow* InputModule::gWindow = NULL;
double InputModule::mousePos[] = { 0, 0 };


void InputModule::Intiate(GameWindow* window_) {
	gWindow = window_;

	glfwSetInputMode(gWindow->GetWindow(), GLFW_STICKY_KEYS, GL_TRUE);	
	glfwSetInputMode(gWindow->GetWindow(), GLFW_CURSOR, GLFW_CURSOR_NORMAL);

	glfwSetCursorPos(gWindow->GetWindow(), gWindow->GetWidth() / 2, gWindow->GetHeight() / 2);
}

void InputModule::CheckInput(){
	glfwGetCursorPos(gWindow->GetWindow(), &mousePos[0], &mousePos[1]);
	glfwSetCursorPos(gWindow->GetWindow(), gWindow->GetWidth() / 2, gWindow->GetHeight() / 2);
}

bool InputModule::IsPressed(int key){
	return glfwGetKey(gWindow->GetWindow(), key) == GLFW_PRESS;
}

double * InputModule::GetMousePos(){
	return mousePos;
}
