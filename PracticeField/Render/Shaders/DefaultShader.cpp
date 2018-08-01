#include "DefaultShader.h"
#include "../../Bases/Camera.h"
#include "../../Lights/DirectionalLight.h"
#include <glm/gtc/type_ptr.hpp>
#include "../RenderData.h"
#include "../../Mesh/Mesh.h"

#include "../Texture.h"

DefaultShader::DefaultShader() {
	filePathVertex = "default.vert";
	filePathFragment = "default.frag";

	LoadProgram(filePathVertex, "", filePathFragment);
}

DefaultShader::~DefaultShader() {
}

void DefaultShader::Initialize() {
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

void DefaultShader::SetUniforms(RenderData * renderData_, glm::mat4 modelMat_, glm::mat4 mvpMat_) {
	Use();

	SetUniformMVP(renderData_, modelMat_, mvpMat_);

	SetUniformDlight(renderData_->camera, (*renderData_->lights)[0]);
}

void DefaultShader::SetUniformMVP(RenderData* renderData_, glm::mat4 modelMat_, glm::mat4 mvpMat_) {
	glUniformMatrix4fv(id_matrice.mvp, 1, GL_FALSE, glm::value_ptr(mvpMat_));
	glUniformMatrix4fv(id_matrice.view, 1, GL_FALSE, glm::value_ptr(renderData_->camera->Vmatrix()));
	glUniformMatrix4fv(id_matrice.model, 1, GL_FALSE, glm::value_ptr(modelMat_));
}

void DefaultShader::ApplyTexture(Mesh * mesh_) {
	if (mesh_->textures.size() <= 0) {
		glUniform1i(id_diffuse.count, 0);
	} else {
		GLuint diffuseNr = 0;
		GLuint specularNr = 1;

		glUniform1i(id_diffuse.count, 1);
		for (GLuint i = 0; i < mesh_->textures.size(); i++) {
			GLuint textureId;
			switch (mesh_->textures[i]->type) {
			case TextureType_Diffuse:
				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, mesh_->textures[i]->textureId);
				diffuseNr++;
				break;

			case TextureType_Specular:
				glActiveTexture(GL_TEXTURE1);
				glBindTexture(GL_TEXTURE_2D, mesh_->textures[i]->textureId);
				specularNr++;
				break;

			case TextureType_Normals:

				break;

			case TextureType_DiffuseTransparent:
				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, mesh_->textures[i]->textureId);
				break;
			}

		}
	}
}

void DefaultShader::SetUniformDlight(Camera* cam_, BaseLight* dLight_) {	
	glm::vec4 CameraSpace_dLightPos = cam_->Vmatrix() * glm::vec4(dLight_->position, 0);
	glUniform3f(id_dLight.direction, CameraSpace_dLightPos.x, CameraSpace_dLightPos.y, CameraSpace_dLightPos.z);
	glUniform3f(id_dLight.color, dLight_->color.x, dLight_->color.y, dLight_->color.z);
	glUniform1f(id_dLight.power, dLight_->intensity);
	glUniform1f(id_dLight.shadowMap, dLight_->shadowData.depthMapTextureId);
	glUniformMatrix4fv(id_dLight.lightSpaceMatrix, 1, GL_FALSE, glm::value_ptr(dLight_->lightSpaceMatrix));

	glActiveTexture(GL_TEXTURE0 + TEXTURE_IDX_SHADOWMAP);
	glBindTexture(GL_TEXTURE_2D, dLight_->shadowData.depthMapTextureId);	
}

