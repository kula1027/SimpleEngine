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
	for (int loop = 0; loop < SphereRenderer::horiDivision; loop++) {
		idxPosition[loop] = new int[SphereRenderer::horiDivision];
	}
	Mesh** dividedMeshes = MeshModifier::DivideByAngle(mesh, vertDivision, idxPosition);	

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

	int vCount = 0;	
	for (GLuint loop = 0; loop < meshModel->meshes->size(); loop++) {
		Mesh* processingMesh = meshModel->meshes->at(loop);
		vCount += processingMesh->vertices.size();

		ApplyTexture(processingMesh);
						
		//ax+by+cz+d = 0에서 y가 dividedMeshDisks[loop]->upCenter.y일때 xz평면에서의 직선의 방정식
		float a = cuttingPlane->normalVector.x;
		float by = cuttingPlane->normalVector.y * dividedMeshDisks[loop]->upCenter.y/*dividedMeshDisks[loop]->upCenter.y*/;
		float c = cuttingPlane->normalVector.z;
		float d = cuttingPlane->d;		
		float dirDist = abs(by + d) / (sqrtf(a * a + c * c));

		glBindVertexArray(processingMesh->VAO);
		if (dirDist > dividedMeshDisks[loop]->radius) {//Draw All
			glDrawElements(
				GL_TRIANGLES,
				processingMesh->GetVertexIdxCount(),
				GL_UNSIGNED_INT,
				NULL
			);
		}else {
			vec2 dirPlane = glm::vec2(-cuttingPlane->normalVector.x, -cuttingPlane->normalVector.z);
			float angleRef = Calculator::OrientedAngle(normalize(dirPlane));

			float angleRange = acosf(dirDist / dividedMeshDisks[loop]->radius);
			float angleFrom = angleRef - angleRange;
			float angleTo = angleRef + angleRange;

			angleFrom = angleFrom / (2 * Calculator::PI) * horiDivision;
			int idxFrom = (int)(angleFrom );			
			//angleFrom ~ angleTo를 제외한 부분을 렌더링한다.
			glDrawElements(
				GL_TRIANGLES,
				idxPosition[loop][idxFrom] * 3,
				GL_UNSIGNED_INT,
				NULL
			);

			angleTo = angleTo / (2 * Calculator::PI) * horiDivision;
			GLuint idxTo = idxPosition[loop][(int)angleTo];
			GLuint idxCount = idxPosition[loop][horiDivision - 1] - idxTo;
			glDrawElements(
				GL_TRIANGLES,
				idxCount * 3,
				GL_UNSIGNED_INT,
				(GLvoid*)(idxTo * sizeof(Triangle))
			);
		}
		

		if (cuttingPlane->normalVector.y > 0) {
			
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
