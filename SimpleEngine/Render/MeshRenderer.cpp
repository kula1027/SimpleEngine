#include "MeshRenderer.h"

#include <Mesh/MeshModel.h>
#include <Lights/LightsBundle.h>
#include <FilePooler.h>

#include <Shaders/ShaderManager.h>
#include <Shaders/ShaderBundle.h>
#include <Render/RenderMaterial/RenderMaterial.h>


MeshRenderer::MeshRenderer() {
	
}


MeshRenderer::MeshRenderer(MeshModel * meshModel_) {	
	if (meshModel_ != NULL) {
		
		SetMeshModel(meshModel_);
	} else {
		DebugLog("MeshRenderer Init parameter NULL");
	}
}

MeshRenderer::~MeshRenderer() {
}


void MeshRenderer::RenderMesh() {
	for (GLuint loop = 0; loop < meshModel->meshes->size(); loop++) {
		Mesh* processingMesh = meshModel->meshes->at(loop);

		processingMesh->renderMaterial->ApplyTexture();

		glBindVertexArray(processingMesh->VAO);

		glDrawElements(
			GL_TRIANGLES,
			processingMesh->GetIdxCount(),
			GL_UNSIGNED_INT,
			NULL
		);
	}
}

void MeshRenderer::RenderMesh_NoTexture() {
	for (GLuint loop = 0; loop < meshModel->meshes->size(); loop++) {
		Mesh* processingMesh = meshModel->meshes->at(loop);

		glBindVertexArray(processingMesh->VAO);

		glDrawElements(
			GL_TRIANGLES,
			processingMesh->GetIdxCount(),
			GL_UNSIGNED_INT,
			NULL
		);
	}
}

void MeshRenderer::SetMeshModel(MeshModel * meshModel_) {
	meshModel = meshModel_;
}

MeshModel * MeshRenderer::GetMeshModel() {
	return meshModel;
}

BaseShader * MeshRenderer::GetShader() {
	return NULL;
}


void MeshRenderer::ComputeMatrices(Camera* camera_) {	
	mvpMatrix = camera_->VPmatrix() * transform->GetModelMatrix();
}

glm::mat4 MeshRenderer::Mmatrix() {
	return transform->GetModelMatrix();
}

glm::mat4 MeshRenderer::MVPmatrix() {
	return mvpMatrix;
}

void MeshRenderer::OnAttachedToObject(EngineObject* obj_) {
	BaseComponent::OnAttachedToObject(obj_);

	transform = obj_->GetTransform();
	Scene::RegisterRenderer(this);
}
