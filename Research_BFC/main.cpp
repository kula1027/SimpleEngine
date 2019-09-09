#include "SimpleEngine.h"
#include "Scenes/Sc_ColorLayer.h"

int main() {
	SimpleEngine::Initialize(1536, 864, "Test Scene");
	SimpleEngine::LoadScene(new Sc_ColorLayer());
	SimpleEngine::Begin();

	return 0;
}