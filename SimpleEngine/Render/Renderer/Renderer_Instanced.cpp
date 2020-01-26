#include "Renderer_Instanced.h"

#include <Shaders/BaseShader.h>
#include <FilePooler.h>
#include <Mesh/MeshModel.h>

#include <Bases/BasesBundle.h>
#include <Light/LightsBundle.h>
#include <Light/LightManager.h>
#include <Shaders/ShaderManager.h>
#include <Render/RenderMaterial/RenderMaterial.h>

Renderer_Instanced::Renderer_Instanced() {	
	renderType = RenderType_Forward;
	shader = ShaderManager::GetShader("Forward/forward_inst");

}


Renderer_Instanced::~Renderer_Instanced() {

}


void Renderer_Instanced::InitInstanced() {
	int meshCount = meshModel->meshes->size();

	int childCount = this->transform->GetChildCount();
	std::vector<glm::mat4> vecModelMats;
	
	for (int loop = 0; loop < childCount; loop++) {
		vecModelMats.push_back(this->transform->GetChildAt(loop)->GetModelMatrix());
	}

	for (int loop = 0; loop < meshCount; loop++) {
		Mesh* currentMesh = meshModel->meshes->at(loop);

		glBindVertexArray(currentMesh->VAO);

		glGenBuffers(1, &currentMesh->instanceVBO);
		glBindBuffer(GL_ARRAY_BUFFER, currentMesh->instanceVBO);

		glBufferData(GL_ARRAY_BUFFER, transform->GetChildCount() * sizeof(glm::mat4), &vecModelMats[0], GL_STATIC_DRAW);

		glEnableVertexAttribArray(4);
		glVertexAttribPointer(4, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)0);
		glVertexAttribDivisor(4, 1);

		glEnableVertexAttribArray(4 + 1);
		glVertexAttribPointer(4 + 1,
			4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)(sizeof(glm::vec4)));
		glVertexAttribDivisor(4 + 1, 1);

		glEnableVertexAttribArray(4 + 2);
		glVertexAttribPointer(4 + 2, 
			4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)(2 * sizeof(glm::vec4)));
		glVertexAttribDivisor(4 + 2, 1);

		glEnableVertexAttribArray(4 + 3);
		glVertexAttribPointer(4 + 3,
			4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)(3 * sizeof(glm::vec4)));
		glVertexAttribDivisor(4 + 3, 1);
	}
}

void Renderer_Instanced::RenderMesh_NoTexture() {
	for (GLuint loop = 0; loop < meshModel->meshes->size(); loop++) {
		Mesh* processingMesh = meshModel->meshes->at(loop);		

		glBindVertexArray(processingMesh->VAO);

		glDrawElementsInstanced(GL_TRIANGLES, processingMesh->triangles.size() * 3, GL_UNSIGNED_INT, 0, this->transform->GetChildCount());
	}
}


void Renderer_Instanced::RenderMesh() {
	shader->SetUniforms(this);
	int dLightCount = LightManager::Inst()->directionalLights.size();
	for (int loop = 0; loop < dLightCount; loop++) {
		if (LightManager::Inst()->directionalLights[loop]->castShadow) {
			LightManager::Inst()->directionalLights[loop]->BindShadowMap();
		}
	}

	for (GLuint loop = 0; loop < meshModel->meshes->size(); loop++) {
		Mesh* processingMesh = meshModel->meshes->at(loop);

		processingMesh->renderMaterial->ApplyTexture();

		glBindVertexArray(processingMesh->VAO);

		glDrawElementsInstanced(GL_TRIANGLES, processingMesh->triangles.size() * 3, GL_UNSIGNED_INT, 0, this->transform->GetChildCount());
	}
}