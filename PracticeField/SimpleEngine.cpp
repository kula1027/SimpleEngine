#include "SimpleEngine.h"

#include <gl\glew.h>
#include "ObjectPool.h"
#include "GameWindow.h"
#include "Scene.h"
#include "InputModule.h"
#include "Time.h"

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
	SetVsyncMode(false);

	currentScene = new Scene();
	currentScene->Load();

	do { 
		Time::Tick();
		Time::PrintRenderTime(3);

		InputModule::CheckInput();		
		
		currentScene->UpdateObjects();
		
	//	currentScene->RenderObjects();
		currentScene->RenderObjects();
		
		glfwSwapBuffers(GameWindow::GetWindow());

		glfwPollEvents();		
	} while (GameWindow::ShouldClose() && !InputModule::IsPressed(GLFW_KEY_ESCAPE));
}

void SimpleEngine::SetVsyncMode(bool isOn){
	if (isOn) {
		std::cout << "Vsync On" << std::endl;
		glfwSwapInterval(1);
	}else{
		std::cout << "Vsync Off" << std::endl;
		glfwSwapInterval(0);
	}
}

void SimpleEngine::UpdateObjects(){	
	
}

void SimpleEngine::RenderObjects(){
	
}

SimpleEngine::~SimpleEngine() {
	glfwTerminate();
}
