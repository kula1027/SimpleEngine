#include "InstancedShader.h"
#include <Bases/Camera.h>
#include <Lights/DirectionalLight.h>
#include <glm/gtc/type_ptr.hpp>


InstancedShader::InstancedShader() {
	filePathVertex = "trans_inst_nocull.vert";
	filePathFragment = "trans_inst_nocull.frag";

	LoadProgram(filePathVertex, "", filePathFragment);
}


InstancedShader::~InstancedShader() {
}

void InstancedShader::Initialize() {	
	id_matrice.vp = GetUniformLocation("VP");
	id_matrice.view = GetUniformLocation("V");

	id_diffuse.count = GetUniformLocation("texCountDiff");
	id_diffuse.id = GetUniformLocation("texture_diffuse");
	id_specular.count = GetUniformLocation("texCountSpec");
	id_specular.id = GetUniformLocation("texture_specular");

	id_dLight.direction = GetUniformLocation("directionalLight0.direction");
	id_dLight.color = GetUniformLocation("directionalLight0.color");
	id_dLight.power = GetUniformLocation("directionalLight0.power");
	id_dLight.lightSpaceMatrix = GetUniformLocation("directionalLight0.lightSpaceMatrix");
	id_dLight.shadowMap = GetUniformLocation("directionalLight0.shadowMap");

	id_pLight.position = GetUniformLocation("pointLight0.position_worldspace");
	id_pLight.color = GetUniformLocation("pointLight0.color");
	id_pLight.power = GetUniformLocation("pointLight0.power");

	Use();
	glUniform1i(id_dLight.shadowMap, TEXTURE_IDX_SHADOWMAP);
	glUniform1i(id_diffuse.id, 0);
	glUniform1i(id_specular.id, 1);
}

//void InstancedShader::SetUniforms(RenderData * renderData_, glm::mat4 modelMat_, glm::mat4 mvpMat_) {
//	Use();
//
//	glUniformMatrix4fv(id_matrice.vp, 1, GL_FALSE, glm::value_ptr(renderData_->camera->VPmatrix()));
//	glUniformMatrix4fv(id_matrice.view, 1, GL_FALSE, glm::value_ptr(renderData_->camera->Vmatrix()));
//
//	SetUniformDlight(renderData_->camera, (*renderData_->lights)[0]);
//}

