#include "Renderer_Instanced.h"

#include <Shaders/BaseShader.h>
#include <FilePooler.h>
#include <Mesh/MeshModel.h>

#include <Bases/BasesBundle.h>
#include <Lights/LightsBundle.h>
#include <Shaders/ShaderManager.h>

Renderer_Instanced::Renderer_Instanced() {	
	shader = ShaderManager::GetShader("Forward/forward_inst");
}


Renderer_Instanced::~Renderer_Instanced() {
}

//
//void Renderer_Instanced::Render(RenderData* renderData_) {
//
//	//	shader->SetUniforms(renderData_, modelMatrix, mvpMatrix);
//
//	for (GLuint loop = 0; loop < meshModel->meshes->size(); loop++) {
//		Mesh* processingMesh = meshModel->meshes->at(loop);
//
//		shader->ApplyTexture(processingMesh->textures);
//
//		glBindVertexArray(processingMesh->VAO);
//
//		glDrawElementsInstanced(GL_TRIANGLES, processingMesh->triangles.size() * 3, GL_UNSIGNED_INT, 0, childTransformCount);
//	}
//
//	glBindVertexArray(0);
//	glBindTexture(GL_TEXTURE_2D, 0);
//}

void Renderer_Instanced::InitInstanced() {
	int meshCount = meshModel->meshes->size();

	for (int loop = 0; loop < meshCount; loop++) {
		Mesh* currentMesh = meshModel->meshes->at(loop);

		glBindVertexArray(currentMesh->VAO);

		glGenBuffers(1, &currentMesh->instanceVBO);
		glBindBuffer(GL_ARRAY_BUFFER, currentMesh->instanceVBO);

		//glBufferData(GL_ARRAY_BUFFER, transform->GetChildCount() * sizeof(glm::mat4), &matriceModel[0], GL_STATIC_DRAW);

		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)0);
		glVertexAttribDivisor(0, 1);

		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 
			4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)(sizeof(glm::vec4)));
		glVertexAttribDivisor(1, 1);

		glEnableVertexAttribArray(2);
		glVertexAttribPointer(2, 
			4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)(2 * sizeof(glm::vec4)));
		glVertexAttribDivisor(2, 1);

		glEnableVertexAttribArray(3);
		glVertexAttribPointer(3,
			4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)(3 * sizeof(glm::vec4)));
		glVertexAttribDivisor(3, 1);
	}
}