#include "BaseRenderer.h"

#include "../Mesh/MeshModel.h"
#include "../Lights/LightsBundle.h"
#include "../FilePooler.h"

BaseRenderer::BaseRenderer() {
	castShadow = false;
	cullingEnabled = true;
	lineDrawEnabled = false;
}


BaseRenderer::BaseRenderer(MeshModel * meshModel_) {
	castShadow = true;
	cullingEnabled = true;
	lineDrawEnabled = false;

	SetMeshModel(meshModel_);
}

BaseRenderer::BaseRenderer(Transform* transform_) {
	transform = transform_;
	castShadow = true;
	cullingEnabled = true;
	lineDrawEnabled = false;
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


void BaseRenderer::SetTransform(Transform* transform_) {
	transform = transform_;
}

void BaseRenderer::ComputeMatrix(Camera* camera_) {
	modelMatrix = transform->GetMatrix4();
	mvpMatrix = camera_->VPmatrix() * modelMatrix;
}