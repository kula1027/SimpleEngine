#include "ShaderDeferredGeo.h"
#include <glm/gtc/type_ptr.hpp>


ShaderDeferredGeo::ShaderDeferredGeo() {
	filePath = "Deferred/deferred_geo";
	
	LoadProgram(filePath);

	BindCameraUBO();

	id_matrice.model = GetUniformLocation("M");
}


ShaderDeferredGeo::~ShaderDeferredGeo() {
}

void ShaderDeferredGeo::SetModelMat(glm::mat4 modelMat_) {	
	glUniformMatrix4fv(id_matrice.model, 1, GL_FALSE, glm::value_ptr(modelMat_));
}
