#include "SimpleEngine.h"
#include "Scenes/Sc_SpeedTest.h"

int main() {
	SimpleEngine::Initialize(1536, 864, "Test Scene");
	SimpleEngine::LoadScene(new Sc_SpeedTest());
	SimpleEngine::Begin();

	return 0;
}