#include "OutlineRenderer.h"

#include "../Bases/BasesBundle.h"
#include "../Mesh/MeshModel.h"
#include "Texture.h"
#include "../Lights/LightsBundle.h"
#include "../FilePooler.h"
#include <glm/gtc/matrix_transform.hpp>

OutlineRenderer::OutlineRenderer()
{
}


OutlineRenderer::~OutlineRenderer()
{
}

void OutlineRenderer::SetAdditionalShaderData(BaseShader * shader_)
{
	outlineShader = FilePooler::LoadShader(DefaultVS_Outline, DefaultFS_Outline);

	outlineShader->Use();
	outline.id_color = outlineShader->GetUniformLocation("outlineColor");
	outline.id_thickness = outlineShader->GetUniformLocation("thickness");

	glUniform1f(outline.id_thickness, outline.thickness);
	glUniform3f(outline.id_color, outline.color.x, outline.color.y, outline.color.z);
}

void OutlineRenderer::Render(RenderData* renderData_){
	if (outline.draw) {
		glEnable(GL_STENCIL_TEST);
		glStencilFunc(GL_ALWAYS, 1, 0xFF);//stencil 버퍼에 항상 1로 업데이트
		glStencilMask(0xFF);//and mask
	}

	shader->SetUniforms(renderData_, modelMatrix, mvpMatrix);

	/*glUniform3f(id_pLight.position, lights_[1]->position.x, lights_[1]->position.y, lights_[1]->position.z);
	glUniform3f(id_pLight.color, lights_[1]->color.x, lights_[1]->color.y, lights_[1]->color.z);
	glUniform1f(id_pLight.power, lights_[1]->intensity);*/


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
