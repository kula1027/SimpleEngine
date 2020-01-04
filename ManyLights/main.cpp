#include "SimpleEngine.h"
#include "SceneManyLights.h"

int main() {
	SimpleEngine::Initialize(1536, 864, "Many Lights");
	SimpleEngine::LoadScene(new SceneManyLights());
	SimpleEngine::Begin();

	return 0;
}