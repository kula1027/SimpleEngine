#include "SphereRenderer.h"

#include "MeshModel.h"
#include "MeshModifier.h"
#include <iostream>
#include <math.h>

void SphereRenderer::CalculateBoudingSphere(){

}

SphereRenderer::SphereRenderer(){
	
}


SphereRenderer::~SphereRenderer(){

}

float SphereRenderer::GetBoundingRadius(){
	return boundingRadius;
}

glm::vec3 SphereRenderer::GetBoundingCenter() {
	return boundingCenter;
}

int** SphereRenderer::idxPosition;//TODO
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
	
	glm::vec3 bCenterHori = glm::vec3(
		boundingCenter.x,
		0,
		boundingCenter.z
	);

	float largestHori = -1;
	float largestVert = -1;
	for (int loop = 0; loop < mesh->vertices.size(); loop++) {
		float dist = 0;

		glm::vec3 hori = glm::vec3(
			mesh->vertices[loop].position.x,
			0,
			mesh->vertices[loop].position.z
		);				
		dist = glm::distance(hori, bCenterHori);
		if (dist > largestHori) {
			largestHori = dist;
		}

		glm::vec3 vert = glm::vec3(
			0,
			mesh->vertices[loop].position.y,
			mesh->vertices[loop].position.z
		);
		dist = glm::distance(vert, bCenterHori);
		if (dist > largestVert) {
			largestVert = dist;
		}
	}

	boundingRadius = largestHori;

	radiusVert = largestVert;
	radiusHori = largestHori;

	idxPosition = new int*[divisionCount];
	for (int loop = 0; loop < horiDivision; loop++) {
		idxPosition[loop] = new int[SphereRenderer::horiDivision];
	}
	Mesh** dividedMeshes = MeshModifier::DivideByAngle(mesh, divisionCount, idxPosition);

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

	glm::vec3 dirCam =cam_->transform->position - (boundingCenter + transform->position);	
	float angleVertical = glm::dot(dirCam, glm::vec3(0, 1, 0));	
	float angleHorizontal = glm::dot(glm::normalize(glm::vec3(dirCam.x, 0, dirCam.z)), glm::vec3(0, 0, 1));
	if (dirCam.x < 0) {
		angleHorizontal = -angleHorizontal + 1;
	}else {
		angleHorizontal += 3;
	}	

	float horizonRange = GetHorizontalAngleRange(dirCam);
	horizonRange = (horizonRange / (glm::pi<float>() * 2)) * 4; // 0<= ~ <= 4
	float hRange[] = {//hRange[0]부터 [1]까지 draw
		fmodf((angleHorizontal - horizonRange) + 4, 4.0f),// 0 <= ~ < 4
		fmodf((angleHorizontal + horizonRange) + 4, 4.0f)
	};
	hRange[0] = hRange[0] / 4 * horiDivision; //0 <= ~ < horiDivision
	hRange[1] = hRange[1] / 4 * horiDivision;
	
	int a = (int)hRange[0] - 1;
	int b = (int)hRange[1];

	int vCount = 0;
	for (GLuint loop = 0; loop < divisionCount; loop++) {
		Mesh* processingMesh = meshModel->meshes->at(loop);
		vCount += processingMesh->vertices.size();

		int startIdx = 0;
		int tCount = 0;
		if((int)hRange[0] >= (horiDivision / 2)){
			startIdx = idxPosition[loop][(int)hRange[0]];
			tCount = idxPosition[loop][horiDivision - 1] - startIdx;	
		}						

		ApplyTexture(processingMesh);

		glBindVertexArray(processingMesh->VAO);
		
		glDrawElements(
			GL_TRIANGLES,
			tCount * 3,
			GL_UNSIGNED_INT,
			(GLvoid*)(startIdx * sizeof(Triangle))
		);
		glDrawElements(
			GL_TRIANGLES,
			idxPosition[loop][(horiDivision / 2) - 1] * 3,
			GL_UNSIGNED_INT,
			NULL
		);

		/*glDrawElements(
			GL_TRIANGLES,
			processingMesh->triangles.size() * 3,
			GL_UNSIGNED_INT,
			NULL
		);*/
	}


	glBindVertexArray(0);
	glBindTexture(GL_TEXTURE_2D, 0);

	RestoreDrawingMode();
}

float SphereRenderer::GetHorizontalAngleRange(glm::vec3 dirCam_) {
	dirCam_ -= boundingCenter;

	glm::vec2 dirCamNorm = glm::normalize(glm::vec2(dirCam_.x, dirCam_.z));

	float squareX = dirCam_.x * dirCam_.x;
	float squareZ = dirCam_.z * dirCam_.z;
	float squareR = radiusHori * radiusHori;

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
