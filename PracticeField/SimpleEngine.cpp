#include "SimpleEngine.h"

#include <assimp\postprocess.h>
#include <assimp\cimport.h>
#include <assimp\scene.h>

void SimpleEngine::Initialize(int width, int height, const char* name){
	if (!glfwInit()){
		fprintf(stderr, "Failed to initialize GLFW\n");
		getchar();	

		return;
	}

	GameWindow::Init(width, height, "Practice Field");

	glewExperimental = true;
	if (glewInit() != GLEW_OK) {
		fprintf(stderr, "Failed to initialize GLEW\n");
		getchar();
		glfwTerminate();
		return;
	}

	InputModule::Init();		
}

void SimpleEngine::Begin() {
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_ALWAYS);

	currentScene = new Scene();
	currentScene->Load();

	do {
		Time::Tick();

		InputModule::CheckInput();		
		
		currentScene->UpdateObjects();
		
		currentScene->RenderObjects();
		
		glfwSwapBuffers(GameWindow::GetWindow());

		glfwPollEvents();		
	} while (GameWindow::ShouldClose());
}

void SimpleEngine::UpdateObjects(){	
	
}

void SimpleEngine::RenderObjects(){
	
}

SimpleEngine::~SimpleEngine() {
	glfwTerminate();
}
