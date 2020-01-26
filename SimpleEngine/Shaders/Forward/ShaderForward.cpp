#include "ShaderForward.h"
#include <Bases/Camera.h>
#include <Light/LightsBundle.h>
#include <Shaders/ShaderDef.h>
#include <Render/Renderer/MeshRenderer.h>
#include <glm/gtc/type_ptr.hpp>

#include <Mesh/Mesh.h>

#include <Debugger/SP_Debugger.h>

#include <Render/Texture.h>

ShaderForward::ShaderForward() {
	filePath = "Forward/forward";	

	LoadProgram(filePath);

	BindLightUBO();
	BindCameraUBO();	

	id_matrice.mvp = GetUniformLocation("MVP");
	id_matrice.model = GetUniformLocation("M");

	id_diffuse.id = GetUniformLocation("texture_diffuse");
	id_specular.id = GetUniformLocation("texture_specular");

	Use();	
	glUniform1i(id_diffuse.id, 0);	
	glUniform1i(id_specular.id, 1);		
	glUniform1i(GetUniformLocation("shadowMap"), ShadowMapID);
}

ShaderForward::~ShaderForward() {
}

void ShaderForward::SetUniforms(MeshRenderer * renderer_) {
	Use();
	
	glUniformMatrix4fv(id_matrice.mvp, 1, GL_FALSE, glm::value_ptr(renderer_->MVPmatrix()));	
	glUniformMatrix4fv(id_matrice.model, 1, GL_FALSE, glm::value_ptr(renderer_->Mmatrix()));	
}