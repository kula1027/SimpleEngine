#include "Renderer.h"

#include "Camera.h"
#include "Transform.h"
#include "MeshModel.h"
#include "Texture.h"
#include "Lights.h"
#include "FileManager.h"
#include <glm/gtc/matrix_transform.hpp>


Renderer::Renderer(){
	castShadow = true;
	cullingEnabled = true;
	isStatic = true;
}

Renderer::Renderer(Transform* transform_){
	transform = transform_;
	castShadow = true;
	cullingEnabled = true;
	isStatic = true;
}

Renderer::~Renderer(){
	free(shader);
	free(outlineShader);
}


void Renderer::SetTransform(Transform* transform_){
	transform = transform_;
}


void Renderer::SetShader(Shader* shader_){
	shader = shader_;

	id_matrice.mvp = shader->GetUniformLocation("MVP");
	id_matrice.view = shader->GetUniformLocation("V");
	id_matrice.model = shader->GetUniformLocation("M");
	
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
	outlineShader = FileManager::LoadShader(DefaultVS_Outline, DefaultFS_Outline);

	outlineShader->Use();
	outline.id_color = outlineShader->GetUniformLocation("outlineColor");
	outline.id_thickness = outlineShader->GetUniformLocation("thickness");

	glUniform1f(outline.id_thickness, outline.thickness);
	glUniform3f(outline.id_color, outline.color.x, outline.color.y, outline.color.z);
}

void Renderer::SetDefaultShader(){
	shader = FileManager::LoadShader(DefualtVS, DefaultFS);

	id_matrice.mvp = shader->GetUniformLocation("MVP");
	id_matrice.view = shader->GetUniformLocation("V");
	id_matrice.model = shader->GetUniformLocation("M");

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
	outlineShader = FileManager::LoadShader(DefaultVS_Outline, DefaultFS_Outline);

	outlineShader->Use();
	outline.id_color = outlineShader->GetUniformLocation("outlineColor");
	outline.id_thickness = outlineShader->GetUniformLocation("thickness");

	glUniform1f(outline.id_thickness, outline.thickness);
	glUniform3f(outline.id_color, outline.color.x, outline.color.y, outline.color.z);
}

void Renderer::SetMeshModel(MeshModel * meshModel_){
	meshModel = meshModel_;

	if (meshModel->isSetup == false) {
		int meshCount = meshModel->meshes->size();
		for (int loop = 0; loop < meshCount; loop++) {
			Mesh* currentMesh = meshModel->meshes->at(loop);

			glGenVertexArrays(1, &currentMesh->VAO);
			glBindVertexArray(currentMesh->VAO);

			glGenBuffers(1, &currentMesh->VBO);
			glBindBuffer(GL_ARRAY_BUFFER, currentMesh->VBO);
			
			if (isStatic) {
				glBufferData(GL_ARRAY_BUFFER, currentMesh->vertices.size() * sizeof(Vertex), &currentMesh->vertices[0], GL_STATIC_DRAW);
			}
			else {
				glBufferData(GL_ARRAY_BUFFER, currentMesh->vertices.size() * sizeof(Vertex), &currentMesh->vertices[0], GL_DYNAMIC_DRAW);
			}

			// Vertex Positions
			glEnableVertexAttribArray(AttrLoc_Position);
			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)0);//현재 bind된 buffer의 내용을 기술된 attribute대로 vao에 박아넣음
			// Vertex Normals
			glEnableVertexAttribArray(AttrLoc_Normal);
			glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, Normal));
			// Vertex Texture Coords
			glEnableVertexAttribArray(AttrLoc_TexCoord);
			glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, TexCoords));

			glGenBuffers(1, &currentMesh->EBO);
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, currentMesh->EBO);
			if (isStatic) {
				glBufferData(GL_ELEMENT_ARRAY_BUFFER, currentMesh->triangles.size() * sizeof(Triangle),
					&currentMesh->triangles[0], GL_STATIC_DRAW);
			}
			else {
				glBufferData(GL_ELEMENT_ARRAY_BUFFER, currentMesh->triangles.size() * sizeof(Triangle),
					&currentMesh->triangles[0], GL_DYNAMIC_DRAW);
			}

			glBindVertexArray(0);
		}

		meshModel->isSetup = true;
	}
}

void Renderer::Render(Camera * cam_, std::vector<BaseLight*> lights_){
	if (cullingEnabled) {
		glEnable(GL_CULL_FACE);
	}else {
		glDisable(GL_CULL_FACE);
	}

	modelMatrix = ComputeModelMatrix(this->transform);
	mvpMatrix = cam_->VPmatrix() * modelMatrix;

	shader->Use();
	
	glUniformMatrix4fv(id_matrice.mvp, 1, GL_FALSE, glm::value_ptr(mvpMatrix));
	glUniformMatrix4fv(id_matrice.view, 1, GL_FALSE, glm::value_ptr(cam_->Vmatrix()));
	glUniformMatrix4fv(id_matrice.model, 1, GL_FALSE, glm::value_ptr(modelMatrix));

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
		glEnable(GL_STENCIL_TEST);
		glStencilFunc(GL_ALWAYS, 1, 0xFF);//stencil 버퍼에 항상 1로 업데이트
		glStencilMask(0xFF);//and mask
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

		outlineShader->Use();
		
		glUniformMatrix4fv(outlineShader->GetUniformLocation("MVP"), 1, GL_FALSE, glm::value_ptr(mvpMatrix));

		for (GLuint loop = 0; loop < meshModel->meshes->size(); loop++) {
			Mesh* processingMesh = meshModel->meshes->at(loop);
			glBindVertexArray(processingMesh->VAO);

			glDrawElements(GL_TRIANGLES, processingMesh->triangles.size() * 3, GL_UNSIGNED_INT, 0);
		}	

		glDisable(GL_STENCIL_TEST);
	}

	glStencilMask(0xFF);
	glBindVertexArray(0);
	glBindTexture(GL_TEXTURE_2D, 0);
}

void Renderer::RenderShadowMap(BaseLight* light_){	
	if (castShadow) {
		modelMatrix = ComputeModelMatrix(this->transform);

		glUniformMatrix4fv(light_->modelMatrixId, 1, GL_FALSE, glm::value_ptr(modelMatrix));

		for (GLuint loop = 0; loop < meshModel->meshes->size(); loop++) {
			Mesh* processingMesh = meshModel->meshes->at(loop);

			glBindVertexArray(processingMesh->VAO);

			glDrawElements(GL_TRIANGLES, processingMesh->triangles.size() * 3, GL_UNSIGNED_INT, 0);
		}

		glBindVertexArray(0);
	}
}

glm::mat4 Renderer::ComputeModelMatrix(Transform* transform_){
	glm::mat4 mMat = glm::mat4(1.0);
	mMat = glm::translate(mMat, transform_->position);
	mMat = glm::rotate(mMat, glm::radians(transform_->rotation.x), glm::vec3(1, 0, 0));
	mMat = glm::rotate(mMat, glm::radians(transform_->rotation.y), glm::vec3(0, 1, 0));
	mMat = glm::rotate(mMat, glm::radians(transform_->rotation.z), glm::vec3(0, 0, 1));
	mMat = glm::scale(mMat, transform_->scale);
	
	return mMat;
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