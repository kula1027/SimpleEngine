#include <iostream>

#include "SimpleEngine.h"
#include "Scenes/SceneBundle.h"

int main() {	
	SimpleEngine::Initialize(1280, 720, "Simple Engine");
	SimpleEngine::LoadScene(new TestScene2());
	SimpleEngine::Begin();

	return 0;
}
