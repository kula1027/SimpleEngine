#include "PreCullingRenderer.h"
#include "PreCullingRenderer_Split.h"
#include "../Mesh/MeshModifier.h"

#include <ImaginaryFigures/ImaginaryFigures.h>
#include <Mesh/MeshBundle.h>
#include <Bases/BasesBundle.h>
#include <iostream>

PreCullingRenderer::PreCullingRenderer()
{
}


PreCullingRenderer::~PreCullingRenderer()
{
}


void PreCullingRenderer::CalculateBoudingSphere() {
}

void PreCullingRenderer::SetMeshModel(MeshModel* meshModel_) {
	meshModel = meshModel_;

	Mesh* mesh = meshModel->meshes->at(0);

	if (mesh->isSetup)return;

	renderMaterial = new PcRendererMaterial();

	renderMaterial->horiDivision = 64;
	renderMaterial->vertDivision = 6;

	//Get Bounding(AA)
	ImaginaryCube* boundingBox = ImaginaryCube::GetBoundingBox(mesh);
	renderMaterial->boundingSphere = ImaginarySphere::GetBoundingSphere(mesh, boundingBox->center);

	//Divide by Angle
	renderMaterial->idxPosition = new int*[renderMaterial->vertDivision];
	for (int loop = 0; loop < renderMaterial->vertDivision; loop++) {
		renderMaterial->idxPosition[loop] = new int[renderMaterial->horiDivision];
	}
	renderMaterial->verticalIdxer = MeshModifier::RearrangeMeshByNormal(mesh, renderMaterial->vertDivision, renderMaterial->horiDivision, renderMaterial->idxPosition);

	//Set Disks
	renderMaterial->dividedMeshDisks = new ImaginaryDisk*[renderMaterial->vertDivision];
	float height = renderMaterial->boundingSphere->radius * 2 / renderMaterial->vertDivision;
	float angleItv = SimpleMath::PI / renderMaterial->vertDivision;
	for (int loop = 0; loop < renderMaterial->vertDivision; loop++) {
		glm::vec3 upCenter = glm::vec3(0, renderMaterial->boundingSphere->radius - height * loop, 0);
		float rad = sinf(loop * angleItv) * renderMaterial->boundingSphere->radius;
		if (rad < sinf((loop + 1) * angleItv) * renderMaterial->boundingSphere->radius) {
			rad = sinf((loop + 1) * angleItv) * renderMaterial->boundingSphere->radius;
		}
		renderMaterial->dividedMeshDisks[loop] = new ImaginaryDisk(upCenter, height, rad);
	}

	mesh->Setup();
	
	delete boundingBox;
}

void PreCullingRenderer::Render(RenderData* renderData_) {
	shader->Use();

	shader->SetUniforms(renderData_, modelMatrix, mvpMatrix);

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

	vector<GLsizei> listDrawCount;
	vector<GLvoid*> listDrawIndice;
	int drawCount = 0;

	int vCount = 0;
	int drawingFaces = 0;
	int frontFaceCount = 0;
	int totalFaceCount = 0;
	
	Mesh* processingMesh = meshModel->meshes->at(0);
	glBindVertexArray(processingMesh->VAO);
	shader->ApplyTexture(processingMesh->textures);
	for (GLuint loop = 0; loop < renderMaterial->vertDivision; loop++) {								
		float diskCenterY;
		if (cuttingPlane->normalVector.y > 0) {
			diskCenterY = renderMaterial->dividedMeshDisks[loop]->upCenter.y;
		}
		else {
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

		//angleFrom ~ angleTo를 제외한 부분을 렌더링한다.
		if (by + d > 0 && dirDist > renderMaterial->dividedMeshDisks[loop]->radius) {
			listDrawIndice.push_back((GLvoid*)(renderMaterial->verticalIdxer[0][loop] * sizeof(Triangle)));
			listDrawCount.push_back(renderMaterial->verticalIdxer[1][loop] * 3);

			/*glDrawElements(
				GL_TRIANGLES,
				renderMaterial->verticalIdxer[1][loop] * 3,
				GL_UNSIGNED_INT,
				(GLvoid*)(renderMaterial->verticalIdxer[0][loop] * sizeof(Triangle))
			);*/
		}
		else if (by + d < 0 && dirDist > renderMaterial->dividedMeshDisks[loop]->radius) {
			//draw nothing
		}
		else {
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

				listDrawIndice.push_back((GLvoid*)(renderMaterial->verticalIdxer[0][loop] * sizeof(Triangle)));
				listDrawCount.push_back(faceIdxLeft * 3);

				GLuint faceIdxRight = renderMaterial->idxPosition[loop][idxRight - 1];
				GLuint faceCount = renderMaterial->idxPosition[loop][renderMaterial->horiDivision - 1] - faceIdxRight;

				int drawIdx = renderMaterial->verticalIdxer[0][loop] + faceIdxRight;
				listDrawIndice.push_back((GLvoid*)(drawIdx * sizeof(Triangle)));
				listDrawCount.push_back(faceCount * 3 );

				/*glDrawElements(
					GL_TRIANGLES,
					faceIdxLeft * 3,
					GL_UNSIGNED_INT,
					(GLvoid*)(renderMaterial->verticalIdxer[0][loop] * sizeof(Triangle))
				); 
				glDrawElements(
					GL_TRIANGLES,
					faceCount * 3,
					GL_UNSIGNED_INT,
					(GLvoid*)(drawIdx * sizeof(Triangle))
				);*/
			}
			else {
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
				if (idxRight < 1) {
					faceIdxRight = 0;
				}
				else {
					faceIdxRight = renderMaterial->idxPosition[loop][idxRight - 1];
				}

				GLuint faceIdxLeft = renderMaterial->idxPosition[loop][idxLeft];
				GLuint faceCount = faceIdxLeft - faceIdxRight;

				int drawIdx = renderMaterial->verticalIdxer[0][loop] + faceIdxRight;
				listDrawIndice.push_back((GLvoid*)(drawIdx * sizeof(Triangle)));
				listDrawCount.push_back(faceCount * 3);

				/*glDrawElements(
					GL_TRIANGLES,
					faceCount * 3,
					GL_UNSIGNED_INT,
					(GLvoid*)(drawIdx * sizeof(Triangle))
				);*/
			}
		}

		if (InputModule::IsPressed(GLFW_KEY_KP_0)) {
			frontFaceCount += CalcFrontFaceCount(processingMesh, renderMaterial->targetCamTr->position);
			totalFaceCount += processingMesh->triangles.size();
		}
	}

	//glMultiDrawElements(GL_TRIANGLES, &listDrawCount[0], GL_UNSIGNED_INT, &listDrawIndice[0], listDrawIndice.size());
	for (int loop = 0; loop < listDrawIndice.size(); loop++) {
		glDrawElements(
			GL_TRIANGLES,
			listDrawCount[loop],
			GL_UNSIGNED_INT,
			listDrawIndice[loop]
		);
	}

	if (InputModule::IsPressed(GLFW_KEY_KP_0)) {
		cout << transform->engineObject->name << " : " << endl
			<< "Drawing Face Count: " << drawingFaces << endl
			<< "Real BackFace Count: " << frontFaceCount << endl
			<< "Total Face Count: " << totalFaceCount << endl
			<< endl;
	}

	glBindVertexArray(0);
	glBindTexture(GL_TEXTURE_2D, 0);

	delete cuttingPlane;
}

ImaginaryPlane* PreCullingRenderer::CalcCuttingPlane(glm::vec3 dirCam_) {

	float sqrRadius = renderMaterial->boundingSphere->radius * renderMaterial->boundingSphere->radius;

	float lengToPlane = sqrRadius / glm::length(dirCam_);

	return new ImaginaryPlane(dirCam_, glm::normalize(dirCam_) * -lengToPlane);
}
int PreCullingRenderer::CalcFrontFaceCount(Mesh * mesh_, glm::vec3 vPosition) {
	int backFaceCount = 0;
	for (int loop = 0; loop < mesh_->faceData.size(); loop++) {
		glm::vec3 dirVtoFace = vPosition - mesh_->faceData[loop].position;
		if (glm::dot(dirVtoFace, mesh_->faceData[loop].normal) > 0) {
			backFaceCount++;
		}
	}

	return backFaceCount;
}
//+dirCam_{ x = -0.00314605236 y = -1.43170261 z = 20.9344559 ... }	glm::tvec3<float, 0>
