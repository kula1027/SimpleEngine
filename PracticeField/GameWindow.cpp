#include "GameWindow.h"



GameWindow::GameWindow(int w_, int h_, const char* name_){
	glfwWindowHint(GLFW_SAMPLES, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	width = w_;
	height = h_;

	window = glfwCreateWindow(width, height, name_, NULL, NULL);

	glfwMakeContextCurrent(window);
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

bool GameWindow::ShouldClose(){
	return !glfwWindowShouldClose(window);
}

GameWindow::~GameWindow(){
}
