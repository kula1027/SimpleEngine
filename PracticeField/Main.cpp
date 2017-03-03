#include <iostream>

#include "SimpleEngine.h"

static SimpleEngine simpleEngine;
int main() {	
	simpleEngine.Initialize(1280, 720, "Practice Field");
	simpleEngine.Begin();

	return 0;
}
