#include "GameWindow.h"

GLFWwindow* GameWindow::window;
int GameWindow::width;
int GameWindow::height;
bool GameWindow::isFocused;

void GameWindow::Init(int w_, int h_, const char* name_){
	glfwWindowHint(GLFW_SAMPLES, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	width = w_;
	height = h_;

	window = glfwCreateWindow(width, height, name_, NULL, NULL);

	glfwMakeContextCurrent(window);

	glfwSetWindowFocusCallback(window, GameWindow::FocusCallBack);
	glfwFocusWindow(window);
	isFocused = true;
}

void GameWindow::FocusCallBack(GLFWwindow* window_, int focused) {
	if (focused) {		
		isFocused = true;
	}
	else {
		isFocused = false;
	}
}

int GameWindow::GetWidth(){
	return width;
}

int GameWindow::GetHeight(){
	return height;
}

GLFWwindow * GameWindow::GetWindow(){
	return window;
}

bool GameWindow::IsFocused() {
	return isFocused;
}

bool GameWindow::ShouldClose(){
	return !glfwWindowShouldClose(window);
}

