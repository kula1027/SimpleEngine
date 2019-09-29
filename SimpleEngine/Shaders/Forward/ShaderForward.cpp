#include "ShaderForward.h"
#include <Bases/Camera.h>
#include <Lights/LightsBundle.h>
#include <Render/MeshRenderer.h>
#include <glm/gtc/type_ptr.hpp>

#include "../../Mesh/Mesh.h"

#include <Debugger/SP_Debugger.h>

#include <Render/Texture.h>

ShaderForward::ShaderForward() {
	filePathVertex = "Forward/forward.vert";
	filePathFragment = "Forward/forward.frag";

	LoadProgram(filePathVertex, "", filePathFragment);
}

ShaderForward::ShaderForward(string vs, string fs) {
	filePathVertex = vs;
	filePathFragment = fs;

	LoadProgram(filePathVertex, "", filePathFragment);
}

ShaderForward::~ShaderForward() {
}

void ShaderForward::Initialize() {
	id_matrice.mvp = GetUniformLocation("MVP");
	id_matrice.view = GetUniformLocation("V");
	id_matrice.model = GetUniformLocation("M");

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

void ShaderForward::SetUniforms(Camera * camera_, MeshRenderer * renderer_, std::vector<BaseLight*>* lights_) {
	Use();
	
	glUniformMatrix4fv(id_matrice.mvp, 1, GL_FALSE, glm::value_ptr(renderer_->MVPmatrix()));
	glUniformMatrix4fv(id_matrice.view, 1, GL_FALSE, glm::value_ptr(camera_->Vmatrix()));
	glUniformMatrix4fv(id_matrice.model, 1, GL_FALSE, glm::value_ptr(renderer_->Mmatrix()));

	SetUniformDlight(camera_, lights_->at(0));

}

void ShaderForward::SetUniformDlight(Camera* cam_, BaseLight* dLight_) {	
	glm::vec4 CameraSpace_dLightPos = cam_->Vmatrix() * glm::vec4(dLight_->GetPosition(), 0);
	glUniform3f(id_dLight.direction, CameraSpace_dLightPos.x, CameraSpace_dLightPos.y, CameraSpace_dLightPos.z);
	glUniform3f(id_dLight.color, dLight_->GetColor().x, dLight_->GetColor().y, dLight_->GetColor().z);
	glUniform1f(id_dLight.power, dLight_->GetIntensity());
	glUniform1f(id_dLight.shadowMap, dLight_->shadowData.depthMapTextureId);
	glUniformMatrix4fv(id_dLight.lightSpaceMatrix, 1, GL_FALSE, glm::value_ptr(dLight_->lightSpaceMatrix));

	glActiveTexture(GL_TEXTURE0 + TEXTURE_IDX_SHADOWMAP);
	glBindTexture(GL_TEXTURE_2D, dLight_->shadowData.depthMapTextureId);	
}

