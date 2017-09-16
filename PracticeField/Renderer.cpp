#include "Renderer.h"

#include "Camera.h"
#include "Transform.h"
#include "MeshModel.h"
#include "Texture.h"
#include "Lights.h"

#include <glm/gtc/matrix_transform.hpp>


Renderer::Renderer()
{
}

Renderer::Renderer(Transform* transform_){
	transform = transform_;
}

Renderer::~Renderer(){
	free(shader);
}


void Renderer::SetReferences(Transform* transform_, MeshModel* meshModel_){
	meshModel = meshModel_;
	transform = transform_;
}


void Renderer::SetShader(Shader* shader_){
	shader = shader_;

	mvpMatrixID = shader->GetUniformLocation("MVP");
	viewMatrixID = shader->GetUniformLocation("V");
	modelMatrixID = shader->GetUniformLocation("M");

	difTexCountID = shader->GetUniformLocation("texCountDiff");
	
	id_dLight.direction = shader->GetUniformLocation("directionalLight0.direction");
	id_dLight.color = shader->GetUniformLocation("directionalLight0.color");
	id_dLight.power = shader->GetUniformLocation("directionalLight0.power");	
	id_dLight.lightSpaceMatrix = shader->GetUniformLocation("directionalLight0.lightSpaceMatrix");
	id_dLight.shadowMap = shader->GetUniformLocation("directionalLight0.shadowMap");

	id_pLight.position = shader->GetUniformLocation("pointLight0.position_worldspace");
	id_pLight.color = shader->GetUniformLocation("pointLight0.color");
	id_pLight.power = shader->GetUniformLocation("pointLight0.power");
}

void Renderer::Render(Camera * cam_, std::vector<BaseLight*> lights_){
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
	
	glUniform1i(id_dLight.shadowMap, 10);
	glActiveTexture(GL_TEXTURE10);
	glBindTexture(GL_TEXTURE_2D, lights_[0]->shadowData.depthMapTextureId);	
	
	glUniform3f(id_pLight.position, lights_[1]->position.x, lights_[1]->position.y, lights_[1]->position.z);
	glUniform3f(id_pLight.color, lights_[1]->color.x, lights_[1]->color.y, lights_[1]->color.z);
	glUniform1f(id_pLight.power, lights_[1]->intensity);

	for (GLuint loop = 0; loop < meshModel->meshes->size(); loop++) {
		Mesh* processingMesh = meshModel->meshes->at(loop);

		ApplyTexture(processingMesh);

		// Draw mesh
		glBindVertexArray(processingMesh->VAO);

		glDrawElements(GL_TRIANGLES, processingMesh->triangles.size() * 3, GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);
	}

	glBindTexture(GL_TEXTURE_2D, 0);
}

void Renderer::RenderShadowMap(BaseLight* light_){	
	modelMatrix = glm::translate(glm::mat4(1.0), transform->position);
	modelMatrix = glm::scale(modelMatrix, transform->scale);
	glUniformMatrix4fv(light_->modelMatrixId, 1, GL_FALSE, glm::value_ptr(modelMatrix));

	for (GLuint loop = 0; loop < meshModel->meshes->size(); loop++) {
		Mesh* processingMesh = meshModel->meshes->at(loop);

		glBindVertexArray(processingMesh->VAO);

		glDrawElements(GL_TRIANGLES, processingMesh->triangles.size() * 3, GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);
	}
}


void Renderer::ComputeModelMatrix(Camera * cam){	
	modelMatrix = glm::translate(glm::mat4(1.0), transform->position);
	modelMatrix = glm::scale(modelMatrix, transform->scale);
	mvpMatrix = cam->VPmatrix() * modelMatrix;
}

void Renderer::ApplyTexture(Mesh* processingMesh_){
	if (processingMesh_->textures.size() <= 0) {
		glUniform1i(difTexCountID, 0);
	}
	else {
		GLuint diffuseNr = 0;
		GLuint specularNr = 0;

		glUniform1i(difTexCountID, 1);

		for (GLuint i = 0; i < processingMesh_->textures.size(); i++)
		{
			glActiveTexture(GL_TEXTURE0 + i); // Activate proper texture unit before binding
											  // Retrieve texture number (the N in diffuse_textureN)
			stringstream ss;
			string number;
			string name = processingMesh_->textures[i]->typeName;
			if (name == "texture_diffuse")
				ss << diffuseNr++; // Transfer GLuint to stream
			else if (name == "texture_specular")
				ss << specularNr++; // Transfer GLuint to stream		

									//cout << name << endl;

			number = ss.str();

			glBindTexture(GL_TEXTURE_2D, processingMesh_->textures[i]->textureId);
			glUniform1i(shader->GetUniformLocation((GLchar*)(name + number).c_str()), i);			
		}
	}
	glActiveTexture(GL_TEXTURE0);
}