#include "ShaderDeferredGeo.h"



ShaderDeferredGeo::ShaderDeferredGeo() {
	filePathVertex = "Deferred/deferred_geo.vert";
	filePathFragment = "Deferred/deferred_geo.frag";

	LoadProgram(filePathVertex, "", filePathFragment);
}


ShaderDeferredGeo::~ShaderDeferredGeo() {
}

void ShaderDeferredGeo::Initialize() {
	id_matrice.vp = GetUniformLocation("VP");
	id_matrice.model = GetUniformLocation("M");
}