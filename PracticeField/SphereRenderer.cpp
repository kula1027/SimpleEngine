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
	horiDivision = 64;
	vertDivision = 16;
}


SphereRenderer::~SphereRenderer() {
	delete boundingSphere;
}

void SphereRenderer::SetMeshModel(MeshModel* meshModel_) {
	meshModel = meshModel_;

	Mesh* mesh = meshModel->meshes->at(0);

	if (mesh->isSetup)return;

	//Get Bounding(AA)
	ImaginaryCube* boundingBox = ImaginaryCube::GetBoundingBox(mesh);	
	boundingSphere = ImaginarySphere::GetBoundingSphere(mesh, boundingBox->center);

	//Divide by Angle
	idxPosition = new int*[vertDivision];
	for (int loop = 0; loop < vertDivision; loop++) {
		idxPosition[loop] = new int[horiDivision];
	}
	Mesh** dividedMeshes = MeshModifier::DivideByAngle(mesh, vertDivision, horiDivision, idxPosition);	

	//Set Disks
	dividedMeshDisks = new ImaginaryDisk*[vertDivision];
	float height = boundingSphere->radius * 2 / vertDivision;
	float angleItv = Calculator::PI / vertDivision;
	for (int loop = 0; loop < vertDivision; loop++) {
		glm::vec3 upCenter = glm::vec3(0, boundingSphere->radius - height * loop, 0);
		float rad = sinf(loop * angleItv) * boundingSphere->radius;
		if (rad < sinf((loop + 1) * angleItv) * boundingSphere->radius) {
			rad = sinf((loop + 1) * angleItv) * boundingSphere->radius;
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

	//glm::vec3 dirCam = cam_->transform->position - (boundingSphere->center + transform->position);
	glm::vec3 dirCam = fakeCamTr->position - (boundingSphere->center + transform->position);

	ImaginaryPlane* cuttingPlane = CalcCuttingPlane(dirCam);
	//cout << cuttingPlane->point.x << " "
	//	<< cuttingPlane->point.y << " "
	//	<< cuttingPlane->point.z << " / "
	//	<< cuttingPlane->normalVector.x << " "
	//	<< cuttingPlane->normalVector.y << " "
	//	<< cuttingPlane->normalVector.z << " "
	//	<< endl;

	int vCount = 0;	
	for (GLuint loop = 0; loop < meshModel->meshes->size(); loop++) {
		Mesh* processingMesh = meshModel->meshes->at(loop);
		glBindVertexArray(processingMesh->VAO);
		vCount += processingMesh->vertices.size();

		ApplyTexture(processingMesh);
		
		float diskCenterY;
		if (cuttingPlane->normalVector.y > 0) {
			diskCenterY = dividedMeshDisks[loop]->upCenter.y;
		} else {
			diskCenterY = dividedMeshDisks[loop]->downCenter.y;
		}
		//ax+by+cz+d = 0에서 y가 dividedMeshDisks[loop]->upCenter.y일때 xz평면에서의 직선의 방정식
		float a = cuttingPlane->normalVector.x;
		float by = cuttingPlane->normalVector.y * diskCenterY;
		float c = cuttingPlane->normalVector.z;
		float d = cuttingPlane->d;		
		float dirDist = abs(by + d) / (sqrtf(a * a + c * c));		
		vec2 dirToPlane = normalize(glm::vec2(-cuttingPlane->normalVector.x, -cuttingPlane->normalVector.z));		
		
		float angleRef = Calculator::OrientedAngle(dirToPlane);			

		float angleRange = acosf(dirDist / dividedMeshDisks[loop]->radius);			
		float angleLeft = angleRef - angleRange;
		float angleRight = angleRef + angleRange;

		//angleFrom ~ angleTo를 제외한 부분을 렌더링한다.
		if (by + d > 0 && dirDist > dividedMeshDisks[loop]->radius) {
			glDrawElements(
				GL_TRIANGLES,
				processingMesh->GetVertexIdxCount(),
				GL_UNSIGNED_INT,
				NULL
			);
		} else if (by + d < 0 && dirDist > dividedMeshDisks[loop]->radius) {
			//draw nothing
		} else {
			if (by + d < 0) {
				angleLeft += Calculator::PI;
				angleRight += Calculator::PI;

				float t = angleLeft;
				angleLeft = angleRight;
				angleRight = t;

				angleLeft = fmod(angleLeft, Calculator::PI * 2);
				angleRight = fmod(angleRight, Calculator::PI * 2);
			}

			int idxLeft = angleLeft / (2 * Calculator::PI) * horiDivision;
			int idxRight = angleRight / (2 * Calculator::PI) * horiDivision;


			if (angleRight < 2 * Calculator::PI && angleLeft > 0 && idxLeft < idxRight) {//2 piece render									
				GLuint faceIdxLeft = idxPosition[loop][idxLeft];
				glDrawElements(
					GL_TRIANGLES,
					faceIdxLeft * 3,
					GL_UNSIGNED_INT,
					NULL
				);

				GLuint faceIdxRight = idxPosition[loop][idxRight - 1];
				GLuint faceCount = idxPosition[loop][horiDivision - 1] - faceIdxRight;
				glDrawElements(
					GL_TRIANGLES,
					faceCount * 3,
					GL_UNSIGNED_INT,
					(GLvoid*)(faceIdxRight * sizeof(Triangle))
				);
			} else {
				int idxLeft = angleLeft / (2 * Calculator::PI) * horiDivision;
				int idxRight = angleRight / (2 * Calculator::PI) * horiDivision;
				//Right에서부터 Left로 렌더
				if (idxRight >= horiDivision) {
					idxRight %= horiDivision;
				}
				if (idxLeft <= 0) {
					idxLeft += horiDivision - 1;
				}

				GLuint faceIdxRight;
				if (idxRight < 1) {
					faceIdxRight = 0;
				} else {
					faceIdxRight = idxPosition[loop][idxRight - 1];
				}

				GLuint faceIdxLeft = idxPosition[loop][idxLeft];
				GLuint faceCount = faceIdxLeft - faceIdxRight;

				glDrawElements(
					GL_TRIANGLES,
					faceCount * 3,
					GL_UNSIGNED_INT,
					(GLvoid*)(faceIdxRight * sizeof(Triangle))
				);
			}
			
			

			
		}		
			
	} 
	

	glBindVertexArray(0);
	glBindTexture(GL_TEXTURE_2D, 0);

	RestoreDrawingMode();
	delete cuttingPlane;
}

ImaginaryPlane* SphereRenderer::CalcCuttingPlane(glm::vec3 dirCam_) {
	
	float sqrRadius = boundingSphere->radius * boundingSphere->radius;	
	
	float lengToPlane = sqrRadius / glm::length(dirCam_);

	return new ImaginaryPlane(dirCam_, glm::normalize(dirCam_) * -lengToPlane);
}
//+dirCam_{ x = -0.00314605236 y = -1.43170261 z = 20.9344559 ... }	glm::tvec3<float, 0>
