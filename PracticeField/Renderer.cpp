#include "Renderer.h"

#include "Camera.h"
#include "Transform.h"
#include "MeshModel.h"
#include "Texture.h"
#include "Lights.h"
#include "FileManager.h"
#include <glm/gtc/matrix_transform.hpp>


Renderer::Renderer(){
	outlineShader = FileManager::LoadShader("outline.vert", "outline.frag");	
	castShadow = true;
	cullingEnabled = true;
}

Renderer::Renderer(Transform* transform_){
	transform = transform_;
	outlineShader = FileManager::LoadShader("outline.vert", "outline.frag");
	castShadow = true;
	cullingEnabled = true;
}

Renderer::~Renderer(){
	free(shader);
}


void Renderer::SetTransform(Transform* transform_){
	transform = transform_;
}


void Renderer::SetShader(Shader* shader_){
	shader = shader_;

	mvpMatrixID = shader->GetUniformLocation("MVP");
	viewMatrixID = shader->GetUniformLocation("V");
	modelMatrixID = shader->GetUniformLocation("M");
	
	id_diffuse.count = shader->GetUniformLocation("texCountDiff");
	id_diffuse.id = shader->GetUniformLocation("texture_diffuse");
	id_specular.count = shader->GetUniformLocation("texCountSpec");
	id_specular.id = shader->GetUniformLocation("texture_specular");
	
	id_dLight.direction = shader->GetUniformLocation("directionalLight0.direction");
	id_dLight.color = shader->GetUniformLocation("directionalLight0.color");
	id_dLight.power = shader->GetUniformLocation("directionalLight0.power");	
	id_dLight.lightSpaceMatrix = shader->GetUniformLocation("directionalLight0.lightSpaceMatrix");
	id_dLight.shadowMap = shader->GetUniformLocation("directionalLight0.shadowMap");
	
	id_pLight.position = shader->GetUniformLocation("pointLight0.position_worldspace");
	id_pLight.color = shader->GetUniformLocation("pointLight0.color");
	id_pLight.power = shader->GetUniformLocation("pointLight0.power");

	shader->Use();
	glUniform1i(id_dLight.shadowMap, TEXTURE_IDX_SHADOWMAP);	
	glUniform1i(id_diffuse.id, 0);
	glUniform1i(id_specular.id, 1);
	
	//Outline
	outline.id_color = outlineShader->GetUniformLocation("outlineColor");
	outline.id_thickness = outlineShader->GetUniformLocation("thickness");

	outlineShader->Use();
	glUniform1f(outline.id_thickness, outline.thickness);
	glUniform3f(outline.id_color, outline.color.x, outline.color.y, outline.color.z);
}

void Renderer::SetMeshModel(MeshModel * meshModel_){
	meshModel = meshModel_;
}

void Renderer::Render(Camera * cam_, std::vector<BaseLight*> lights_){
	if (cullingEnabled) {
		glEnable(GL_CULL_FACE);
	}else {
		glDisable(GL_CULL_FACE);
	}

	ComputeModelMatrix(cam_);

	shader->Use();

	glUniformMatrix4fv(mvpMatrixID, 1, GL_FALSE, glm::value_ptr(mvpMatrix));
	glUniformMatrix4fv(viewMatrixID, 1, GL_FALSE, glm::value_ptr(cam_->Vmatrix()));
	glUniformMatrix4fv(modelMatrixID, 1, GL_FALSE, glm::value_ptr(modelMatrix));

	glm::vec4 CameraSpace_dLightPos = cam_->Vmatrix() * glm::vec4(lights_[0]->position, 0);
	glUniform3f(id_dLight.direction, CameraSpace_dLightPos.x, CameraSpace_dLightPos.y, CameraSpace_dLightPos.z);
	glUniform3f(id_dLight.color, lights_[0]->color.x, lights_[0]->color.y, lights_[0]->color.z);
	glUniform1f(id_dLight.power, lights_[0]->intensity);
	glUniform1f(id_dLight.shadowMap, lights_[0]->shadowData.depthMapTextureId);
	glUniformMatrix4fv(id_dLight.lightSpaceMatrix, 1, GL_FALSE, glm::value_ptr(lights_[0]->lightSpaceMatrix));
	
	glActiveTexture(GL_TEXTURE0 + TEXTURE_IDX_SHADOWMAP);
	glBindTexture(GL_TEXTURE_2D, lights_[0]->shadowData.depthMapTextureId);	
	
	/*glUniform3f(id_pLight.position, lights_[1]->position.x, lights_[1]->position.y, lights_[1]->position.z);
	glUniform3f(id_pLight.color, lights_[1]->color.x, lights_[1]->color.y, lights_[1]->color.z);
	glUniform1f(id_pLight.power, lights_[1]->intensity);*/	

	if (outline.draw) {
		glStencilFunc(GL_ALWAYS, 1, 0xFF);//stencil 버퍼에 항상 1로 업데이트
		glStencilMask(0xFF);//and mask
	}
	else {
		glClear(GL_STENCIL_BUFFER_BIT);
		glStencilMask(0x00);
	}

	for (GLuint loop = 0; loop < meshModel->meshes->size(); loop++) {
		Mesh* processingMesh = meshModel->meshes->at(loop);

		ApplyTexture(processingMesh);

		glBindVertexArray(processingMesh->VAO);

		glDrawElements(GL_TRIANGLES, processingMesh->triangles.size() * 3, GL_UNSIGNED_INT, 0);
	}

	if (outline.draw) {
		glStencilFunc(GL_NOTEQUAL, 1, 0xFF);//stencil buffer값이 1과 다르면 그린다(pass).
		glStencilMask(0x00);//stencil buffer에 쓰지는 않는다.
	//	glDisable(GL_DEPTH_TEST);

		outlineShader->Use();
		
		glUniformMatrix4fv(outlineShader->GetUniformLocation("MVP"), 1, GL_FALSE, glm::value_ptr(mvpMatrix));

		for (GLuint loop = 0; loop < meshModel->meshes->size(); loop++) {
			Mesh* processingMesh = meshModel->meshes->at(loop);
			glBindVertexArray(processingMesh->VAO);

			glDrawElements(GL_TRIANGLES, processingMesh->triangles.size() * 3, GL_UNSIGNED_INT, 0);
		}	
	}

	glStencilMask(0xFF);
	glBindVertexArray(0);
	glBindTexture(GL_TEXTURE_2D, 0);

}

void Renderer::RenderShadowMap(BaseLight* light_){	
	if (castShadow) {
		modelMatrix = glm::translate(glm::mat4(1.0), transform->position);
		modelMatrix = glm::rotate(modelMatrix, glm::radians(transform->rotation.x), glm::vec3(1, 0, 0));
		modelMatrix = glm::rotate(modelMatrix, glm::radians(transform->rotation.y), glm::vec3(0, 1, 0));
		modelMatrix = glm::rotate(modelMatrix, glm::radians(transform->rotation.z), glm::vec3(0, 0, 1));
		modelMatrix = glm::scale(modelMatrix, transform->scale);

		glUniformMatrix4fv(light_->modelMatrixId, 1, GL_FALSE, glm::value_ptr(modelMatrix));

		for (GLuint loop = 0; loop < meshModel->meshes->size(); loop++) {
			Mesh* processingMesh = meshModel->meshes->at(loop);

			glBindVertexArray(processingMesh->VAO);

			glDrawElements(GL_TRIANGLES, processingMesh->triangles.size() * 3, GL_UNSIGNED_INT, 0);
		}

		glBindVertexArray(0);
	}
}

void Renderer::ComputeModelMatrix(Camera * cam){	
	modelMatrix = glm::translate(glm::mat4(1.0), transform->position);
	modelMatrix = glm::rotate(modelMatrix, glm::radians(transform->rotation.x), glm::vec3(1, 0, 0));
	modelMatrix = glm::rotate(modelMatrix, glm::radians(transform->rotation.y), glm::vec3(0, 1, 0));
	modelMatrix = glm::rotate(modelMatrix, glm::radians(transform->rotation.z), glm::vec3(0, 0, 1));
	modelMatrix = glm::scale(modelMatrix, transform->scale);
	mvpMatrix = cam->VPmatrix() * modelMatrix;
}

void Renderer::ApplyTexture(Mesh* processingMesh_){
	if (processingMesh_->textures.size() <= 0) {
		glUniform1i(id_diffuse.count, 0);		
	}else {
		GLuint diffuseNr = 0;
		GLuint specularNr = 1;

		glUniform1i(id_diffuse.count, 1);
		for (GLuint i = 0; i < processingMesh_->textures.size(); i++)
		{
			GLuint textureId;
			switch (processingMesh_->textures[i]->type) {
			case TextureType_Diffuse:				
				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, processingMesh_->textures[i]->textureId);				
				diffuseNr++;
				break;

			case TextureType_Specular:
				glActiveTexture(GL_TEXTURE1);
				glBindTexture(GL_TEXTURE_2D, processingMesh_->textures[i]->textureId);
				specularNr++;
				break;

			case TextureType_Normals:

				break;

			case TextureType_DiffuseTransparent:
				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, processingMesh_->textures[i]->textureId);
				break;
			}
			
		}
	}

	glActiveTexture(GL_TEXTURE0);	
}