#include "SphereRenderer.h"

#include "ImaginaryPlane.h"
#include "ImaginarySphere.h"
#include "ImaginaryCube.h"
#include "MeshModel.h"
#include "MeshModifier.h"
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

	ImaginaryCube* boundingBox = ImaginaryCube::GetBoundingBox(mesh);	
	boundingSphere = ImaginarySphere::GetBoundingSphere(mesh, boundingBox->center);


	idxPosition = new int*[vertDivision];
	for (int loop = 0; loop < horiDivision; loop++) {
		idxPosition[loop] = new int[SphereRenderer::horiDivision];
	}
	Mesh** dividedMeshes = MeshModifier::DivideByAngle(mesh, vertDivision, idxPosition);
	
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

	float horizonRange = GetTangentLines(dirCam);
	
	int vCount = 0;	
	for (GLuint loop = 0; loop < meshModel->meshes->size(); loop++) {
		Mesh* processingMesh = meshModel->meshes->at(loop);
		vCount += processingMesh->vertices.size();

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

float SphereRenderer::GetTangentLines(glm::vec3 dirCam_) {
	dirCam_ -= boundingSphere->center;

	glm::vec2 dirCamNorm = glm::normalize(glm::vec2(dirCam_.x, dirCam_.z));

	float squareX = dirCam_.x * dirCam_.x;
	float squareZ = dirCam_.z * dirCam_.z;
	float squareR = 1 * 1;

	float val = -(dirCam_.x * dirCam_.z);
	float det = squareX * squareZ - (squareR - squareX) * (squareR - squareZ);
	float denom = squareR - squareX;

	float m0 = (val + sqrt(det)) / denom;
	float m1 = (val - sqrt(det)) / denom;

	float k0 = sqrt(1 / (1 + (m0 * m0)));
	float k1 = sqrt(1 / (1 + (m1 * m1)));
	glm::vec2 adjVector[2] = {
		glm::vec2(k0, m0 * k0),
		glm::vec2(k1, m1 * k1)
	};

	if (glm::dot(dirCamNorm, adjVector[0]) < 0) {
		adjVector[0] *= -1;
	}
	if (glm::dot(dirCamNorm, adjVector[1]) < 0) {
		adjVector[1] *= -1;
	}
	float angleAlpha = acosf(glm::dot(adjVector[0], dirCamNorm));
	/*float angleBeta = 2 * angleAlpha;
	float angleTheta = glm::pi<float>() / 2 - angleAlpha;*/

	return angleAlpha + glm::pi<float>() / 2;

	cout << dirCam_.x << ", "
		<< dirCam_.z << "/ "
		<< adjVector[0].x << ", "
		<< adjVector[0].y << "/ "
		<< adjVector[1].x << ", "
		<< adjVector[1].y << endl;
}
