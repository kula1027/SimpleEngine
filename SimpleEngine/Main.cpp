#include <iostream>

#include "SimpleEngine.h"
#include "Scenes/SceneBundle.h"

int main() {	
	SimpleEngine::Initialize(1536, 864, "Simple Engine");
	SimpleEngine::LoadScene();
	SimpleEngine::Begin();

	return 0;
}