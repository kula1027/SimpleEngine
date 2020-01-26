#include "ShaderForward_Instanced.h"
#include <Render/Texture.h>
#include <glm/gtc/type_ptr.hpp>
#include <Shaders/ShaderDef.h>
#include <Render/Renderer/MeshRenderer.h>


ShaderForward_Instanced::ShaderForward_Instanced() {
	filePath = "Forward/forward_inst";

	LoadProgram(filePath);

	BindLightUBO();
	BindCameraUBO();

	id_matrice.mvp = GetUniformLocation("MVP");
	id_matrice.model = GetUniformLocation("M");

	Use();
	glUniform1i(GetUniformLocation("shadowMap"), ShadowMapID);
}

ShaderForward_Instanced::~ShaderForward_Instanced() {
}

void ShaderForward_Instanced::SetUniforms(MeshRenderer * renderer_) {
	Use();

	glUniformMatrix4fv(id_matrice.mvp, 1, GL_FALSE, glm::value_ptr(renderer_->MVPmatrix()));
	glUniformMatrix4fv(id_matrice.model, 1, GL_FALSE, glm::value_ptr(renderer_->Mmatrix()));
}
