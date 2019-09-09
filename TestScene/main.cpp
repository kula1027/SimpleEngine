#include "SimpleEngine.h"
#include "Scenes/TestScene2.h"
#include "Scenes/TestScene.h"

int main() {
	SimpleEngine::Initialize(1536, 864, "Test Scene");
	SimpleEngine::LoadScene(new TestScene());
	SimpleEngine::Begin();

	return 0;
}