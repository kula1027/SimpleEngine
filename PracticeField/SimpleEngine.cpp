#include "SimpleEngine.h"


void SimpleEngine::Initialize(int width, int height, const char* name){
	if (!glfwInit()){
		fprintf(stderr, "Failed to initialize GLFW\n");
		getchar();	

		return;
	}

	gWindow = new GameWindow(width, height, "Practice Field");

	glewExperimental = true;
	if (glewInit() != GLEW_OK) {
		fprintf(stderr, "Failed to initialize GLEW\n");
		getchar();
		glfwTerminate();
		return;
	}

	InputModule::Intiate(gWindow);		
}

void SimpleEngine::Begin() {
	currentScene = new Scene(gWindow);
	currentScene->Load();
	
	do {
		Time::Tick();

		InputModule::CheckInput();

		currentScene->UpdateObjects();
		
		currentScene->RenderObjects();
		
		glfwSwapBuffers(gWindow->GetWindow());

		glfwPollEvents();		
	} while (gWindow->ShouldClose());
}

void SimpleEngine::UpdateObjects(){	
	
}

void SimpleEngine::RenderObjects(){
	
}

SimpleEngine::~SimpleEngine() {
	glfwTerminate();
}
