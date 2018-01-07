#include "Renderer.h"

#include <chrono>
#include "Camera.h"
#include "Transform.h"
#include "GameObject.h"
#include "MeshModel.h"
#include "Texture.h"
#include "Lights.h"
#include "FileManager.h"
#include "Time.h"
#include <glm/gtc/matrix_transform.hpp>


Renderer::Renderer(){
	castShadow = true;
	cullingEnabled = true;	
	lineDrawEnabled = false;
}

Renderer::Renderer(MeshModel * meshModel_){
	castShadow = true;
	cullingEnabled = true;
	lineDrawEnabled = false;

	SetMeshModel(meshModel_);
}

Renderer::Renderer(Transform* transform_){
	transform = transform_;
	castShadow = true;
	cullingEnabled = true;
	lineDrawEnabled = false;
}

Renderer::~Renderer(){
	free(shader);
	free(outlineShader);
}

void Renderer::SetTransform(Transform* transform_){
	transform = transform_;
}

void Renderer::ComputeMatrix(){
	modelMatrix = transform->GetMatrix4();
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

	int meshCount = meshModel->meshes->size();
	for (int loop = 0; loop < meshCount; loop++) {
		Mesh* currentMesh = meshModel->meshes->at(loop);
		if (!currentMesh->isSetup) {
			currentMesh->Setup();
		}
	}
}

void Renderer::Render(Camera * cam_, std::vector<BaseLight*> lights_) {
	SetDrawingMode();

	if (outline.draw) {
		glEnable(GL_STENCIL_TEST);
		glStencilFunc(GL_ALWAYS, 1, 0xFF);//stencil 버퍼에 항상 1로 업데이트
		glStencilMask(0xFF);//and mask
	}
	
	shader->Use();

	SetUniformMVP(cam_);

	SetUniformDlight(cam_, lights_[0]);

	/*glUniform3f(id_pLight.position, lights_[1]->position.x, lights_[1]->position.y, lights_[1]->position.z);
	glUniform3f(id_pLight.color, lights_[1]->color.x, lights_[1]->color.y, lights_[1]->color.z);
	glUniform1f(id_pLight.power, lights_[1]->intensity);*/


	for (GLuint loop = 0; loop < meshModel->meshes->size(); loop++) {
		Mesh* processingMesh = meshModel->meshes->at(loop);

		ApplyTexture(processingMesh);

		glBindVertexArray(processingMesh->VAO);

		glDrawElements(
			GL_TRIANGLES,
			processingMesh->GetVertexIdxCount(),
			GL_UNSIGNED_INT,
			NULL
		);
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

	RestoreDrawingMode();
}

void Renderer::RenderShadowMap(BaseLight* light_){	
	if (castShadow) {		
		glUniformMatrix4fv(light_->modelMatrixId, 1, GL_FALSE, glm::value_ptr(modelMatrix));

		for (GLuint loop = 0; loop < meshModel->meshes->size(); loop++) {
			Mesh* processingMesh = meshModel->meshes->at(loop);

			glBindVertexArray(processingMesh->VAO);

			glDrawElements(GL_TRIANGLES, processingMesh->triangles.size() * 3, GL_UNSIGNED_INT, 0);
		}

		glBindVertexArray(0);
	}
}

void Renderer::SetupIdx()
{
	/*glBindVertexArray(this->VAO);

	glGenBuffers(samplingDirCount, EBOs); 
	for (int loop = 0; loop < samplingDirCount; loop++) {
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBOs[loop]);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, this->arrangedTriangles[loop].size() * sizeof(Triangle),
			&this->arrangedTriangles[loop][0], GL_STATIC_DRAW);
	}
	
	/*glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, this->triangles.size() * sizeof(Triangle),
		&this->triangles[0], GL_STATIC_DRAW);

	glBindVertexArray(0);*/
}

MeshModel * Renderer::GetMeshModel() {
	return meshModel;
}

void Renderer::SetDrawingMode(){
	if (!cullingEnabled) {
		glDisable(GL_CULL_FACE);
	}
	if (lineDrawEnabled) {
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	}
}

void Renderer::SetUniformDlight(Camera* cam_, BaseLight* dLight){
	glm::vec4 CameraSpace_dLightPos = cam_->Vmatrix() * glm::vec4(dLight->position, 0);
	glUniform3f(id_dLight.direction, CameraSpace_dLightPos.x, CameraSpace_dLightPos.y, CameraSpace_dLightPos.z);
	glUniform3f(id_dLight.color, dLight->color.x, dLight->color.y, dLight->color.z);
	glUniform1f(id_dLight.power, dLight->intensity);
	glUniform1f(id_dLight.shadowMap, dLight->shadowData.depthMapTextureId);
	glUniformMatrix4fv(id_dLight.lightSpaceMatrix, 1, GL_FALSE, glm::value_ptr(dLight->lightSpaceMatrix));

	glActiveTexture(GL_TEXTURE0 + TEXTURE_IDX_SHADOWMAP);
	glBindTexture(GL_TEXTURE_2D, dLight->shadowData.depthMapTextureId);
}

void Renderer::SetUniformMVP(Camera* cam_)
{
	mvpMatrix = cam_->VPmatrix() * modelMatrix;
	glUniformMatrix4fv(id_matrice.mvp, 1, GL_FALSE, glm::value_ptr(mvpMatrix));
	glUniformMatrix4fv(id_matrice.view, 1, GL_FALSE, glm::value_ptr(cam_->Vmatrix()));
	glUniformMatrix4fv(id_matrice.model, 1, GL_FALSE, glm::value_ptr(modelMatrix));
}

void Renderer::RestoreDrawingMode(){
	if (!cullingEnabled) {
		glEnable(GL_CULL_FACE);
	}
	if (lineDrawEnabled) {
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	}
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