#include "SimpleEngine.h"
#include "Scenes/TestScene_Texture.h"

int main() {
	SimpleEngine::Initialize(1536, 864, "Texture Test");
	SimpleEngine::LoadScene(new TestScene_Texture());
	SimpleEngine::Begin();

	return 0;
}