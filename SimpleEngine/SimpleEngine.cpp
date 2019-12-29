#include "SimpleEngine.h"

#include <gl\glew.h>
#include "Bases/BasesBundle.h"
#include "GameWindow.h"

#include <assimp\postprocess.h>
#include <assimp\cimport.h>
#include <assimp\scene.h>

#include <EngineDef.h>

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

	glfwSwapInterval(VSYNC_ON);
	InputModule::Init();		
}

void SimpleEngine::LoadScene(Scene * scene_){
	if (currentScene != NULL) {
		currentScene->Unload();
	}	

	currentScene = scene_;
	currentScene->Load();
}

void SimpleEngine::Begin() {
	do { 
		SP_Time::Tick();

		InputModule::CheckInput();		
		
		currentScene->UpdateScripts();
			
		currentScene->RenderScene();
		
		glfwSwapBuffers(GameWindow::GetWindow());

		glfwPollEvents();		
	} while (GameWindow::ShouldClose() && !InputModule::IsPressed(GLFW_KEY_ESCAPE));
}

Scene * SimpleEngine::GetCurrentScene() {
	return currentScene;
}


SimpleEngine::~SimpleEngine() {
	glfwTerminate();
}
