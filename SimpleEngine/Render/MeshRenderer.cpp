#include "MeshRenderer.h"

#include "../Mesh/MeshModel.h"
#include "../Lights/LightsBundle.h"
#include "../FilePooler.h"

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


#pragma region Forward Rendering
void MeshRenderer::SetRenderModeForward(bool isForward_) {
	isRenderModeForward = isForward_;

	if (isRenderModeForward) {
		SetRenderModeForward(ShaderManager::Inst()->GetShader("Forward/forward"));
	}
}

void MeshRenderer::SetRenderModeForward(BaseShader * sf_) {
	isRenderModeForward = true;
	shaderForward = sf_;	
}

bool MeshRenderer::GetRenderModeForward() {
	return isRenderModeForward;
}

//for forward Rendering
void MeshRenderer::RenderMesh_Forward(Camera * camera_) {
	shaderForward->Use();
	shaderForward->SetUniforms(camera_, this);

	RenderMesh();
}
#pragma endregion



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

void MeshRenderer::SetMeshModel(MeshModel * meshModel_) {
	meshModel = meshModel_;

	int meshCount = meshModel->meshes->size();
	for (int loop = 0; loop < meshCount; loop++) {
		Mesh* currentMesh = meshModel->meshes->at(loop);
		if (!currentMesh->isSetup) {
			currentMesh->Setup();
		}
	}
}

MeshModel * MeshRenderer::GetMeshModel() {
	return meshModel;
}

BaseShader * MeshRenderer::GetShader() {
	return NULL;
}


void MeshRenderer::ComputeMatrices(Camera* camera_) {
	modelMatrix = transform->GetModelMatrix();
	mvpMatrix = camera_->VPmatrix() * modelMatrix;
}

glm::mat4 MeshRenderer::Mmatrix() {
	return modelMatrix;
}

glm::mat4 MeshRenderer::MVPmatrix() {
	return mvpMatrix;
}

void MeshRenderer::OnAttachedToObject(EngineObject* obj_) {
	BaseComponent::OnAttachedToObject(obj_);

	transform = obj_->transform;
	Scene::RegisterRenderer(this);
}
