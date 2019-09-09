#include "BaseRenderer.h"

#include "../Mesh/MeshModel.h"
#include "../Lights/LightsBundle.h"
#include "../FilePooler.h"

BaseRenderer::BaseRenderer() {
	castShadow = false;
}


BaseRenderer::BaseRenderer(MeshModel * meshModel_) {
	castShadow = true;

	SetMeshModel(meshModel_);
}

BaseRenderer::BaseRenderer(Transform* transform_) {
	transform = transform_;
	castShadow = true;
}

BaseRenderer::~BaseRenderer() {
}

void BaseRenderer::Initialize() {	
	shader->Initialize();
}

void BaseRenderer::SetMeshModel(MeshModel * meshModel_) {
	meshModel = meshModel_;

	int meshCount = meshModel->meshes->size();
	for (int loop = 0; loop < meshCount; loop++) {
		Mesh* currentMesh = meshModel->meshes->at(loop);
		if (!currentMesh->isSetup) {
			currentMesh->Setup();
		}
	}
}

MeshModel * BaseRenderer::GetMeshModel() {
	return meshModel;
}


void BaseRenderer::ComputeMatrix(Camera* camera_) {
	modelMatrix = transform->GetMatrix4();
	mvpMatrix = camera_->VPmatrix() * modelMatrix;
}

glm::mat4 BaseRenderer::Mmatrix() {
	return modelMatrix;
}

void BaseRenderer::OnAttachedToObject(EngineObject* obj_) {
	BaseComponent::OnAttachedToObject(obj_);

	transform = obj_->transform;
	Scene::RegisterRenderer(this);
}
