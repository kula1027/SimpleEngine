#include <iostream>

#include "SimpleEngine.h"
#include "TestScene_Texture.h"

int main() {
	SimpleEngine::Initialize(1536, 864, "Simple Engine");
	SimpleEngine::LoadScene(new TestScene_Texture());
	SimpleEngine::Begin();

	return 0;
}