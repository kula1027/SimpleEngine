#include "InstancedRenderer.h"
#include "GameObject.h"
#include "Shader.h"
#include "FileManager.h"
#include "MeshModel.h"

#include "Camera.h"
#include "Lights.h"

InstancedRenderer::InstancedRenderer(){
	castShadow = false;
}


InstancedRenderer::~InstancedRenderer()
{
}


void InstancedRenderer::SetDefaultShader(){
	shader = FileManager::LoadShader(DefaultVS_Ist, DefaultFS);

	id_matrice.vp = shader->GetUniformLocation("VP");
	id_matrice.view = shader->GetUniformLocation("V");

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
}

void InstancedRenderer::Render(Camera * cam_, std::vector<BaseLight*> lights_){
	if (cullingEnabled) {
		glEnable(GL_CULL_FACE);
	}
	else {
		glDisable(GL_CULL_FACE);
	}	

	shader->Use();

	glUniformMatrix4fv(id_matrice.vp, 1, GL_FALSE, glm::value_ptr(cam_->VPmatrix()));
	glUniformMatrix4fv(id_matrice.view, 1, GL_FALSE, glm::value_ptr(cam_->Vmatrix()));	

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

	//glClear(GL_STENCIL_BUFFER_BIT);

	for (GLuint loop = 0; loop < meshModel->meshes->size(); loop++) {
		Mesh* processingMesh = meshModel->meshes->at(loop);

		ApplyTexture(processingMesh);

		glBindVertexArray(processingMesh->VAO);

		glDrawElementsInstanced(GL_TRIANGLES, processingMesh->triangles.size() * 3, GL_UNSIGNED_INT, 0, childTransformCount);
	}

	glBindVertexArray(0);
	glBindTexture(GL_TEXTURE_2D, 0);
}

void InstancedRenderer::SetMeshModel(MeshModel * meshModel_){
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
			} else {
				glBufferData(GL_ARRAY_BUFFER, currentMesh->vertices.size() * sizeof(Vertex), &currentMesh->vertices[0], GL_DYNAMIC_DRAW);
			}

			// Vertex Positions
			glEnableVertexAttribArray(AttrLoc_Position);
			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)0);
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
			glBindBuffer(GL_ARRAY_BUFFER, 0);
		}

		meshModel->isSetup = true;
	}
}

void InstancedRenderer::InitInstanced() {
	int meshCount = meshModel->meshes->size();

	for (int loop = 0; loop < meshCount; loop++) {
		Mesh* currentMesh = meshModel->meshes->at(loop);

		glBindVertexArray(currentMesh->VAO);

		glGenBuffers(1, &currentMesh->instanceVBO);
		glBindBuffer(GL_ARRAY_BUFFER, currentMesh->instanceVBO);
		
		if (isStatic) {
			vector<Transform*>* tChildren = &transform->children;
			childTransformCount = tChildren->size();
			matriceModel = new glm::mat4[childTransformCount];
			for (int loop = 0; loop < childTransformCount; loop++) {
				matriceModel[loop] = ComputeModelMatrix((*tChildren)[loop]);				
			}

			glBufferData(GL_ARRAY_BUFFER, childTransformCount * sizeof(glm::mat4), &matriceModel[0], GL_STATIC_DRAW);
		}

		glEnableVertexAttribArray(AttrLoc_IstMatrix0);
		glVertexAttribPointer(AttrLoc_IstMatrix0, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)0);
		glVertexAttribDivisor(AttrLoc_IstMatrix0, 1);

		glEnableVertexAttribArray(AttrLoc_IstMatrix1);
		glVertexAttribPointer(AttrLoc_IstMatrix1, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)(sizeof(glm::vec4)));
		glVertexAttribDivisor(AttrLoc_IstMatrix1, 1);

		glEnableVertexAttribArray(AttrLoc_IstMatrix2);
		glVertexAttribPointer(AttrLoc_IstMatrix2, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)(2 * sizeof(glm::vec4)));
		glVertexAttribDivisor(AttrLoc_IstMatrix2, 1);

		glEnableVertexAttribArray(AttrLoc_IstMatrix3);
		glVertexAttribPointer(AttrLoc_IstMatrix3, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)(3 * sizeof(glm::vec4)));
		glVertexAttribDivisor(AttrLoc_IstMatrix3, 1);

		glBindVertexArray(0);		
	}

	
}