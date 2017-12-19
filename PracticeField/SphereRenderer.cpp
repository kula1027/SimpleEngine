#include "SphereRenderer.h"

#include "ImaginaryFigures.h"
#include "MeshModel.h"
#include "MeshModifier.h"
#include "Calculator.h"
#include <iostream>
#include <math.h>

void SphereRenderer::CalculateBoudingSphere() {

}

SphereRenderer::SphereRenderer() {
}


SphereRenderer::~SphereRenderer() {
	delete boundingSphere;
}

int** SphereRenderer::idxPosition;//TODO
void SphereRenderer::SetMeshModel(MeshModel* meshModel_) {
	meshModel = meshModel_;

	Mesh* mesh = meshModel->meshes->at(0);
	if (mesh->isSetup)return;

	//Get Bounding(AA)
	ImaginaryCube* boundingBox = ImaginaryCube::GetBoundingBox(mesh);	
	boundingSphere = ImaginarySphere::GetBoundingSphere(mesh, boundingBox->center);

	//Divide by Angle
	idxPosition = new int*[vertDivision];
	for (int loop = 0; loop < horiDivision; loop++) {
		idxPosition[loop] = new int[SphereRenderer::horiDivision];
	}
	Mesh** dividedMeshes = MeshModifier::DivideByAngle(mesh, vertDivision, idxPosition);	

	//Set Disks
	dividedMeshDisks = new ImaginaryDisk*[vertDivision];
	float height = boundingSphere->radius * 2 / vertDivision;
	for (int loop = 0; loop < vertDivision; loop++) {
		glm::vec3 upCenter = glm::vec3(0, boundingSphere->radius - height * loop, 0);
		float rad = sinf(loop * glm::pi<float>() * boundingSphere->radius);
		if (rad < sinf((loop + 1) * glm::pi<float>() * boundingSphere->radius)) {
			rad = sinf((loop + 1) * glm::pi<float>() * boundingSphere->radius);
		}
		dividedMeshDisks[loop] = new ImaginaryDisk(upCenter, height, rad);
	}
	
	//Replace existing mesh w divided meshes
	meshModel->meshes->pop_back();
	for (int loop = 0; loop < vertDivision; loop++) {
		dividedMeshes[loop]->Setup();
		meshModel->meshes->push_back(dividedMeshes[loop]);
	}

	delete mesh;
	delete boundingBox;
}

void SphereRenderer::Render(Camera * cam_, std::vector<BaseLight*> lights_) {
	SetDrawingMode();

	shader->Use();

	SetUniformMVP(cam_);

	SetUniformDlight(cam_, lights_[0]);

	glm::vec3 dirCam = cam_->transform->position - (boundingSphere->center + transform->position);
	dirCam = glm::vec3(10, 10, 10);

	boundingSphere->radius = 5;
	ImaginaryPlane* cuttingPlane = CalcCuttingPlane(dirCam);
	
	int vCount = 0;	
	for (GLuint loop = 0; loop < meshModel->meshes->size(); loop++) {
		Mesh* processingMesh = meshModel->meshes->at(loop);
		vCount += processingMesh->vertices.size();

		ApplyTexture(processingMesh);
				
		//equation on disk y
		vec2 dir = glm::vec2(-cuttingPlane->normalVector.x, -cuttingPlane->normalVector.z);
		float angleRef = Calculator::OrientedAngle(normalize(dir));
		float a = cuttingPlane->normalVector.x;
		float c = cuttingPlane->normalVector.z;
		float d = cuttingPlane->normalVector.y * dividedMeshDisks[loop]->upCenter.y + cuttingPlane->d;
		float dirDist = abs(d) / (sqrtf(a * a) + sqrtf(c * c));
		float angleRange = acosf(dirDist / dividedMeshDisks[loop]->radius);		

		if (cuttingPlane->normalVector.y > 0) {
			
		}

		glBindVertexArray(processingMesh->VAO);

		glDrawElements(
			GL_TRIANGLES,
			processingMesh->GetVertexIdxCount(),
			GL_UNSIGNED_INT,
			NULL
		);
	}	

	glBindVertexArray(0);
	glBindTexture(GL_TEXTURE_2D, 0);

	RestoreDrawingMode();
	delete cuttingPlane;
}

ImaginaryPlane* SphereRenderer::CalcCuttingPlane(glm::vec3 dirCam_) {
	dirCam_ -= boundingSphere->center;
	
	float sqrRadius = boundingSphere->radius * boundingSphere->radius;	

	float lengToPlane = sqrRadius / dirCam_.length();
	
	return new ImaginaryPlane(dirCam_, glm::normalize(dirCam_) * -lengToPlane);
}
