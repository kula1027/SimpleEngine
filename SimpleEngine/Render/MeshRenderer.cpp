#include "MeshRenderer.h"

#include "../Mesh/MeshModel.h"
#include "../Lights/LightsBundle.h"
#include "../FilePooler.h"
#include <Shaders/ShaderBundle.h>

MeshRenderer::MeshRenderer() {
	Initialize(NULL);
}


MeshRenderer::MeshRenderer(MeshModel * meshModel_) {	
	Initialize(meshModel_);
}

MeshRenderer::~MeshRenderer() {
}

void MeshRenderer::Initialize(MeshModel * meshModel_) {	
	castShadow = true;

	if (meshModel_ != NULL) {
		SetMeshModel(meshModel_);
	}	

	shader = new ShaderForward();
	shader->Initialize();
}

void MeshRenderer::RenderMesh(Camera * camera_, std::vector<BaseLight*>* lights_) {
	shader->SetUniforms(camera_, this, lights_);

	RenderMesh();
}

void MeshRenderer::RenderMesh() {
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
	return shader;
}


void MeshRenderer::ComputeMatrix(Camera* camera_) {
	modelMatrix = transform->GetMatrix4();
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
