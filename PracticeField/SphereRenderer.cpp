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
	Renderer::SetMeshModel(meshModel_);
	
	Mesh* mesh = meshModel->meshes->at(0);

	glm::vec3 box_max = glm::vec3(-99, -99, -99);
	glm::vec3 box_min = glm::vec3(99, 99, 99);;
	for (int loop = 0; loop < mesh->vertices.size(); loop++) {
		if (mesh->vertices[loop].Position.x > box_max.x) {
			box_max.x = mesh->vertices[loop].Position.x;
		}
		if (mesh->vertices[loop].Position.y > box_max.y) {
			box_max.y = mesh->vertices[loop].Position.y;
		}
		if (mesh->vertices[loop].Position.z > box_max.z) {
			box_max.z = mesh->vertices[loop].Position.z;
		}

		if (mesh->vertices[loop].Position.x < box_min.x) {
			box_min.x = mesh->vertices[loop].Position.x;
		}
		if (mesh->vertices[loop].Position.y < box_min.y) {
			box_min.y = mesh->vertices[loop].Position.y;
		}
		if (mesh->vertices[loop].Position.z < box_min.z) {
			box_min.z = mesh->vertices[loop].Position.z;
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
			mesh->vertices[loop].Position.x,
			0,
			mesh->vertices[loop].Position.z
		);
		
		float dist = glm::distance(v, c);
		if (dist > largest) {
			largest = dist;
		}
	}

	boundingRadius = largest;

	MeshModifier::DivideVertical(mesh, boundingCenter, 5);

	mesh->Setup();
}
