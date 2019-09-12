#include "DefaultRenderer.h"

#include <chrono>
#include <glm/gtc/matrix_transform.hpp>

#include "Texture.h"

#include "../Bases/BasesBundle.h"
#include "../Mesh/MeshModel.h"
#include "../Lights/LightsBundle.h"
#include "../FilePooler.h"

DefaultRenderer::DefaultRenderer(){
	defaultVS = "default.vert";
	defaultFS = "default.frag";
}

DefaultRenderer::~DefaultRenderer(){

}

void DefaultRenderer::SetShader(BaseShader* shader_){
	shader = shader_;
	
	SetAdditionalShaderData(shader_);
}

void DefaultRenderer::SetShader() {
	SetShader(FilePooler::LoadShader(defaultVS, defaultFS));
}

void DefaultRenderer::Render(RenderData* renderData_) {	
	//shader->SetUniforms(renderData_, modelMatrix, mvpMatrix);

	SetUniformAdditional();	
	for (GLuint loop = 0; loop < meshModel->meshes->size(); loop++) {
		Mesh* processingMesh = meshModel->meshes->at(loop);
	
		shader->ApplyTexture(processingMesh->textures);

		glBindVertexArray(processingMesh->VAO);

		glDrawElements(
			GL_TRIANGLES,
			processingMesh->GetIdxCount(),
			GL_UNSIGNED_INT,
			NULL
		);
	}

	glBindVertexArray(0);
	glBindTexture(GL_TEXTURE_2D, 0);	
}

void DefaultRenderer::RenderShadowMap(BaseLight* light_){	
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