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
	
}

void SphereRenderer::SetMeshModel(MeshModel* meshModel_) {
	meshModel = meshModel_;

	Mesh* mesh = meshModel->meshes->at(0);

	if (mesh->isSetup)return;

	renderMaterial = new SphereRenderMaterial();

	renderMaterial->horiDivision = 64;
	renderMaterial->vertDivision = 16;

	//Get Bounding(AA)
	ImaginaryCube* boundingBox = ImaginaryCube::GetBoundingBox(mesh);	
	renderMaterial->boundingSphere = ImaginarySphere::GetBoundingSphere(mesh, boundingBox->center);

	//Divide by Angle
	renderMaterial->idxPosition = new int*[renderMaterial->vertDivision];
	for (int loop = 0; loop < renderMaterial->vertDivision; loop++) {
		renderMaterial->idxPosition[loop] = new int[renderMaterial->horiDivision];
	}
	Mesh** dividedMeshes = MeshModifier::DivideByAngle(mesh, renderMaterial->vertDivision, renderMaterial->horiDivision, renderMaterial->idxPosition);

	//Set Disks
	renderMaterial->dividedMeshDisks = new ImaginaryDisk*[renderMaterial->vertDivision];
	float height = renderMaterial->boundingSphere->radius * 2 / renderMaterial->vertDivision;
	float angleItv = Calculator::PI / renderMaterial->vertDivision;
	for (int loop = 0; loop < renderMaterial->vertDivision; loop++) {
		glm::vec3 upCenter = glm::vec3(0, renderMaterial->boundingSphere->radius - height * loop, 0);
		float rad = sinf(loop * angleItv) * renderMaterial->boundingSphere->radius;
		if (rad < sinf((loop + 1) * angleItv) * renderMaterial->boundingSphere->radius) {
			rad = sinf((loop + 1) * angleItv) * renderMaterial->boundingSphere->radius;
		}
		renderMaterial->dividedMeshDisks[loop] = new ImaginaryDisk(upCenter, height, rad);
	}
	
	//Replace existing mesh w divided meshes
	meshModel->meshes->pop_back();
	for (int loop = 0; loop < renderMaterial->vertDivision; loop++) {
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
	glm::vec3 dirCam = renderMaterial->targetCamTr->position - (renderMaterial->boundingSphere->center + transform->position);

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
			diskCenterY = renderMaterial->dividedMeshDisks[loop]->upCenter.y;
		} else {
			diskCenterY = renderMaterial->dividedMeshDisks[loop]->downCenter.y;
		}
		//ax+by+cz+d = 0에서 y가 dividedMeshDisks[loop]->upCenter.y일때 xz평면에서의 직선의 방정식
		float a = cuttingPlane->normalVector.x;
		float by = cuttingPlane->normalVector.y * diskCenterY;
		float c = cuttingPlane->normalVector.z;
		float d = cuttingPlane->d;		
		float dirDist = abs(by + d) / (sqrtf(a * a + c * c));		
		vec2 dirToPlane = normalize(glm::vec2(-cuttingPlane->normalVector.x, -cuttingPlane->normalVector.z));		
		
		float angleRef = Calculator::OrientedAngle(dirToPlane);			

		float angleRange = acosf(dirDist / renderMaterial->dividedMeshDisks[loop]->radius);
		float angleLeft = angleRef - angleRange;
		float angleRight = angleRef + angleRange;

		//angleFrom ~ angleTo를 제외한 부분을 렌더링한다.
		if (by + d > 0 && dirDist > renderMaterial->dividedMeshDisks[loop]->radius) {
			glDrawElements(
				GL_TRIANGLES,
				processingMesh->GetVertexIdxCount(),
				GL_UNSIGNED_INT,
				NULL
			);
		} else if (by + d < 0 && dirDist > renderMaterial->dividedMeshDisks[loop]->radius) {
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

			int idxLeft = angleLeft / (2 * Calculator::PI) * renderMaterial->horiDivision;
			int idxRight = angleRight / (2 * Calculator::PI) * renderMaterial->horiDivision;


			if (angleRight < 2 * Calculator::PI && angleLeft > 0 && idxLeft < idxRight) {//2 piece render									
				GLuint faceIdxLeft = renderMaterial->idxPosition[loop][idxLeft];
				glDrawElements(
					GL_TRIANGLES,
					faceIdxLeft * 3,
					GL_UNSIGNED_INT,
					NULL
				);

				GLuint faceIdxRight = renderMaterial->idxPosition[loop][idxRight - 1];
				GLuint faceCount = renderMaterial->idxPosition[loop][renderMaterial->horiDivision - 1] - faceIdxRight;
				glDrawElements(
					GL_TRIANGLES,
					faceCount * 3,
					GL_UNSIGNED_INT,
					(GLvoid*)(faceIdxRight * sizeof(Triangle))
				);
			} else {
				int idxLeft = angleLeft / (2 * Calculator::PI) * renderMaterial->horiDivision;
				int idxRight = angleRight / (2 * Calculator::PI) * renderMaterial->horiDivision;
				//Right에서부터 Left로 렌더
				if (idxRight >= renderMaterial->horiDivision) {
					idxRight %= renderMaterial->horiDivision;
				}
				if (idxLeft <= 0) {
					idxLeft += renderMaterial->horiDivision - 1;
				}

				GLuint faceIdxRight;
				if (idxRight < 1) {
					faceIdxRight = 0;
				} else {
					faceIdxRight = renderMaterial->idxPosition[loop][idxRight - 1];
				}

				GLuint faceIdxLeft = renderMaterial->idxPosition[loop][idxLeft];
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
	
	float sqrRadius = renderMaterial->boundingSphere->radius * renderMaterial->boundingSphere->radius;
	
	float lengToPlane = sqrRadius / glm::length(dirCam_);

	return new ImaginaryPlane(dirCam_, glm::normalize(dirCam_) * -lengToPlane);
}
//+dirCam_{ x = -0.00314605236 y = -1.43170261 z = 20.9344559 ... }	glm::tvec3<float, 0>
