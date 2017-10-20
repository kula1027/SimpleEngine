#include "SphereRenderer.h"

#include "MeshModel.h"
#include "MeshModifier.h"
#include <iostream>


void SphereRenderer::CalculateBoudingSphere(){

}

SphereRenderer::SphereRenderer(){
	
}


SphereRenderer::~SphereRenderer(){

}

float SphereRenderer::GetBoundingRadius(){
	return boundingRadius;
}

glm::vec3 SphereRenderer::GetBoundingCenter()
{
	return boundingCenter;
}

void SphereRenderer::SetMeshModel(MeshModel* meshModel_){
	meshModel = meshModel_;
	
	Mesh* mesh = meshModel->meshes->at(0);
	if (mesh->isSetup)return;

	glm::vec3 box_max = glm::vec3(-99, -99, -99);
	glm::vec3 box_min = glm::vec3(99, 99, 99);;
	for (int loop = 0; loop < mesh->vertices.size(); loop++) {
		if (mesh->vertices[loop].position.x > box_max.x) {
			box_max.x = mesh->vertices[loop].position.x;
		}
		if (mesh->vertices[loop].position.y > box_max.y) {
			box_max.y = mesh->vertices[loop].position.y;
		}
		if (mesh->vertices[loop].position.z > box_max.z) {
			box_max.z = mesh->vertices[loop].position.z;
		}

		if (mesh->vertices[loop].position.x < box_min.x) {
			box_min.x = mesh->vertices[loop].position.x;
		}
		if (mesh->vertices[loop].position.y < box_min.y) {
			box_min.y = mesh->vertices[loop].position.y;
		}
		if (mesh->vertices[loop].position.z < box_min.z) {
			box_min.z = mesh->vertices[loop].position.z;
		}
	}

	boundingCenter = (box_max + box_min) / 2.0f;
	glm::vec3 c = glm::vec3(
		boundingCenter.x,
		0,
		boundingCenter.z
	);

	float largest = -1;
	for (int loop = 0; loop < mesh->vertices.size(); loop++) {
		glm::vec3 v = glm::vec3(
			mesh->vertices[loop].position.x,
			0,
			mesh->vertices[loop].position.z
		);
		
		float dist = glm::distance(v, c);
		if (dist > largest) {
			largest = dist;
		}
	}

	boundingRadius = largest;
	float height = box_max.y - box_min.y;

	Mesh** dividedMeshes = MeshModifier::DivideVertical(mesh, box_min.y, height, divisionCount);

	delete mesh;
	meshModel->meshes->pop_back();

	for (int loop = 0; loop < divisionCount; loop++) {		
		dividedMeshes[loop]->Setup();
		meshModel->meshes->push_back(dividedMeshes[loop]);
	}
}

void SphereRenderer::Render(Camera * cam_, std::vector<BaseLight*> lights_) {
	SetDrawingMode();

	shader->Use();

	SetUniformMVP(cam_);

	SetUniformDlight(cam_, lights_[0]);

	glm::vec3 dirCam = glm::normalize(cam_->transform->position - (boundingCenter + transform->position));
	float angleVertical = glm::dot(dirCam, glm::vec3(0, 1, 0));	

	int segEnd;
	int segStart;
	if (angleVertical <= 0){
		segStart = 0;
		segEnd = (divisionCount + 1) * ((2.0f - abs(angleVertical)) * 0.5f) + 1;
	} else {
		segStart = divisionCount + 1 - (divisionCount + 1) * ((2.0f - abs(angleVertical)) * 0.5f) - 1;
		segEnd = divisionCount;
	}

	//cout << segStart << " / " << segEnd << endl;
	
	if (segStart < 0)segStart = 0;
	if (segEnd > divisionCount)segEnd = divisionCount;

	for (GLuint loop = segStart; loop < segEnd; loop++) {
		Mesh* processingMesh = meshModel->meshes->at(loop);

		ApplyTexture(processingMesh);

		glBindVertexArray(processingMesh->VAO);

		glDrawElements(
			GL_TRIANGLES,
			processingMesh->triangles.size() * 3,
			GL_UNSIGNED_INT,
			NULL
		);
	}

	glBindVertexArray(0);
	glBindTexture(GL_TEXTURE_2D, 0);

	RestoreDrawingMode();
}
