#include "PreCullingColorLayer.h"

#include "../ImaginaryFigures/ImaginaryFigures.h"
#include "../Mesh/MeshBundle.h"
#include "../Bases/BasesBundle.h"
#include <iostream>
#include <math.h>

PreCullingColorLayer::PreCullingColorLayer() {
}


PreCullingColorLayer::~PreCullingColorLayer() {
}

void PreCullingColorLayer::ColorTriangles(glm::vec3 color_, Mesh * mesh_, int from, int count) {
	for (int loop = from; loop < count; loop++) {
		for (int loop2 = 0; loop2 < 3; loop2++) {
			int idx = mesh_->triangles[loop].idx[loop2];
			mesh_->vertices[idx].color = color_;
		}

	}

	mesh_->Setup();

	glBindVertexArray(mesh_->VAO);
}

void PreCullingColorLayer::Render(Camera * cam_, std::vector<BaseLight*> lights_) {
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
	int drawingFaces = 0;
	int frontFaceCount = 0;
	int totalFaceCount = 0;
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

		float angleRef = SimpleMath::OrientedAngle(dirToPlane);

		float angleRange = acosf(dirDist / renderMaterial->dividedMeshDisks[loop]->radius);
		float angleLeft = angleRef - angleRange;
		float angleRight = angleRef + angleRange;

		ColorTriangles(glm::vec3(0, 1, 0), processingMesh, NULL, processingMesh->triangles.size());

		//angleFrom ~ angleTo를 제외한 부분을 렌더링한다.
		if (by + d > 0 && dirDist > renderMaterial->dividedMeshDisks[loop]->radius) {
			ColorTriangles(glm::vec3(0, 0, 1), processingMesh, 0, processingMesh->triangles.size());		
					
		} else if (by + d < 0 && dirDist > renderMaterial->dividedMeshDisks[loop]->radius) {
			//draw nothing
		} else {
			if (by + d < 0) {
				angleLeft += SimpleMath::PI;
				angleRight += SimpleMath::PI;

				float t = angleLeft;
				angleLeft = angleRight;
				angleRight = t;

				angleLeft = fmod(angleLeft, SimpleMath::PI * 2);
				angleRight = fmod(angleRight, SimpleMath::PI * 2);
			}

			int idxLeft = angleLeft / (2 * SimpleMath::PI) * renderMaterial->horiDivision;
			int idxRight = angleRight / (2 * SimpleMath::PI) * renderMaterial->horiDivision;

			if (angleRight < 2 * SimpleMath::PI && angleLeft > 0 && idxLeft < idxRight) {//2 piece render									
				GLuint faceIdxLeft = renderMaterial->idxPosition[loop][idxLeft];

				ColorTriangles(glm::vec3(0, 1, 1), processingMesh, NULL, faceIdxLeft);

				drawingFaces += faceIdxLeft;

				GLuint faceIdxRight = renderMaterial->idxPosition[loop][idxRight - 1];
				GLuint faceCount = renderMaterial->idxPosition[loop][renderMaterial->horiDivision - 1] - faceIdxRight;
		
				//ColorTriangles(glm::vec3(1, 0, 1), processingMesh, faceIdxRight, faceCount);
				cout << "ASD?";
				drawingFaces += faceCount;
			} else {
				int idxLeft = angleLeft / (2 * SimpleMath::PI) * renderMaterial->horiDivision;
				int idxRight = angleRight / (2 * SimpleMath::PI) * renderMaterial->horiDivision;
				//Right에서부터 Left로 렌더
				if (idxRight >= renderMaterial->horiDivision) {
					idxRight %= renderMaterial->horiDivision;
				}
				if (idxLeft <= 0) {
					idxLeft += renderMaterial->horiDivision - 1;
				}

				GLuint faceIdxRight;
				if (idxRight <= 0) {
					faceIdxRight = 0;
				} else {
					faceIdxRight = renderMaterial->idxPosition[loop][idxRight - 1];
				}

				GLuint faceIdxLeft = renderMaterial->idxPosition[loop][idxLeft];
				GLuint faceCount = faceIdxLeft - faceIdxRight;

				ColorTriangles(glm::vec3(1, 1, 1), processingMesh, faceIdxRight, faceCount);

				drawingFaces += faceCount;
			}
			
		}

		glDrawElements(
			GL_TRIANGLES,
			processingMesh->GetIdxCount(),
			GL_UNSIGNED_INT,
			NULL
		);
	}

	glBindVertexArray(0);

	RestoreDrawingMode();
	delete cuttingPlane;
}