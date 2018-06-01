#include <iostream>

#include "SimpleEngine.h"
#include "Scenes/SceneBundle.h"

static SimpleEngine simpleEngine;

int main() {	
	simpleEngine.Initialize(1280, 720, "Simple Engine");
	simpleEngine.LoadScene(new TestScene());
	simpleEngine.Begin();

	return 0;
}
