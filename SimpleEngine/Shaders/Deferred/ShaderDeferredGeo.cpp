#include "ShaderDeferredGeo.h"
#include <glm/gtc/type_ptr.hpp>


ShaderDeferredGeo::ShaderDeferredGeo() {
	filePath = "Deferred/deferred_geo";
	
	LoadProgram(filePath);

	BindCameraUBO();	
}


ShaderDeferredGeo::~ShaderDeferredGeo() {
}