#include "ShaderDeferredGeo.h"



ShaderDeferredGeo::ShaderDeferredGeo() {
	filePath = "Deferred/deferred_geo";
	
	LoadProgram(filePath);
}


ShaderDeferredGeo::~ShaderDeferredGeo() {
}

void ShaderDeferredGeo::Initialize() {
	id_matrice.vp = GetUniformLocation("VP");
	id_matrice.model = GetUniformLocation("M");
}