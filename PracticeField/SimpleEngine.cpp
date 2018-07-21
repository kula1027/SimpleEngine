#include "SimpleEngine.h"

#include <gl\glew.h>
#include "Bases/BasesBundle.h"
#include "GameWindow.h"
#include "Scenes/Scene.h"

#include <assimp\postprocess.h>
#include <assimp\cimport.h>
#include <assimp\scene.h>

Scene* SimpleEngine::currentScene;

void SimpleEngine::Initialize(int width, int height, const char* name){
	if (!glfwInit()){
		fprintf(stderr, "Failed to initialize GLFW\n");
		getchar();	

		return;
	}

	GameWindow::Init(width, height, name);

	glewExperimental = true;
	if (glewInit() != GLEW_OK) {
		fprintf(stderr, "Failed to initialize GLEW\n");
		getchar();
		glfwTerminate();
		return;
	}

	InputModule::Init();		
}

void SimpleEngine::LoadScene(Scene * scene_){
	currentScene = scene_;
	currentScene->Load();
}

void SimpleEngine::LoadScene() {
	currentScene = new Scene();
	currentScene->Load();
}

void SimpleEngine::Begin() {
	do { 
		Time::Tick();

		InputModule::CheckInput();		
		
		currentScene->UpdateObjects();
			
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


SimpleEngine::~SimpleEngine() {
	glfwTerminate();
}
