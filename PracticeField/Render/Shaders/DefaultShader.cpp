#include "DefaultShader.h"



DefaultShader::DefaultShader() {
	dedicatedVS = "default.vert";
	dedicatedFS = "default.frag";

	LoadProgram(dedicatedVS, "", dedicatedFS);
}


DefaultShader::~DefaultShader() {
}
