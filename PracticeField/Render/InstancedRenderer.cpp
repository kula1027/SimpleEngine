#include "InstancedRenderer.h"

#include "Shaders/BaseShader.h"
#include "../FilePooler.h"
#include "../Mesh/MeshModel.h"

#include "../Bases/BasesBundle.h"
#include "../Lights/LightsBundle.h"

InstancedRenderer::InstancedRenderer(){
	castShadow = false;
}


InstancedRenderer::~InstancedRenderer()
{
}


void InstancedRenderer::SetShader(BaseShader * shader_){
	shader = shader_;	
}

void InstancedRenderer::Render(RenderData* renderData_){
	if (cullingEnabled) {
		glEnable(GL_CULL_FACE);
	}
	else {
		glDisable(GL_CULL_FACE);
	}	

	shader->SetUniforms(renderData_, modelMatrix, mvpMatrix);

	for (GLuint loop = 0; loop < meshModel->meshes->size(); loop++) {
		Mesh* processingMesh = meshModel->meshes->at(loop);

		shader->ApplyTexture(processingMesh);

		glBindVertexArray(processingMesh->VAO);

		glDrawElementsInstanced(GL_TRIANGLES, processingMesh->triangles.size() * 3, GL_UNSIGNED_INT, 0, childTransformCount);
	}

	glBindVertexArray(0);
	glBindTexture(GL_TEXTURE_2D, 0);
}

void InstancedRenderer::SetMeshModel(MeshModel * meshModel_){
	meshModel = meshModel_;

	int meshCount = meshModel->meshes->size();
	for (int loop = 0; loop < meshCount; loop++) {
		Mesh* currentMesh = meshModel->meshes->at(loop);
		if (!currentMesh->isSetup) {
			currentMesh->Setup();
		}
	}
}

void InstancedRenderer::InitInstanced() {
	int meshCount = meshModel->meshes->size();

	for (int loop = 0; loop < meshCount; loop++) {
		Mesh* currentMesh = meshModel->meshes->at(loop);

		glBindVertexArray(currentMesh->VAO);

		glGenBuffers(1, &currentMesh->instanceVBO);
		glBindBuffer(GL_ARRAY_BUFFER, currentMesh->instanceVBO);
		
		if (currentMesh->isStaticDraw) {
			vector<Transform*>* tChildren = &transform->children;
			childTransformCount = tChildren->size();
			matriceModel = new glm::mat4[childTransformCount];
			for (int loop = 0; loop < childTransformCount; loop++) {
				matriceModel[loop] = (*tChildren)[loop]->GetMatrix4();
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