#include <iostream>

#include "SimpleEngine.h"

static SimpleEngine simpleEngine;

int main() {	
	simpleEngine.Initialize(1280, 720, "Simple Engine");
	simpleEngine.LoadScene();
	simpleEngine.Begin();

	return 0;
}
