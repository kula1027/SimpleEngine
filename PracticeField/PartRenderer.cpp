#include "PartRenderer.h"

#include "Camera.h"
#include "Transform.h"
#include "MeshModel.h"
#include "Lights.h"
#include "ArrangedMesh.h"

#include <glm/gtc/matrix_transform.hpp>

void PartRenderer::Render(Camera * cam_, std::vector<BaseLight*> lights_){
	ComputeModelMatrix(cam_);

	shader->Use();

	glUniformMatrix4fv(mvpMatrixID, 1, GL_FALSE, glm::value_ptr(mvpMatrix));
	glUniformMatrix4fv(viewMatrixID, 1, GL_FALSE, glm::value_ptr(cam_->Vmatrix()));
	glUniformMatrix4fv(modelMatrixID, 1, GL_FALSE, glm::value_ptr(modelMatrix));

	glm::vec4 CameraSpace_dLightPos = cam_->Vmatrix() * glm::vec4(lights_[0]->position, 0);
	glUniform3f(id_dLight.direction, CameraSpace_dLightPos.x, CameraSpace_dLightPos.y, CameraSpace_dLightPos.z);
	glUniform3f(id_dLight.color, lights_[0]->color.x, lights_[0]->color.y, lights_[0]->color.z);
	glUniform1f(id_dLight.power, lights_[0]->intensity);

	glUniform3f(id_pLight.position, lights_[1]->position.x, lights_[1]->position.y, lights_[1]->position.z);
	glUniform3f(id_pLight.color, lights_[1]->color.x, lights_[1]->color.y, lights_[1]->color.z);
	glUniform1f(id_pLight.power, lights_[1]->intensity);

	for (GLuint loop = 0; loop < meshModel->meshes->size(); loop++) {
		//ArrangedMesh* a = new Mesh();		
		ArrangedMesh* processingMesh = (ArrangedMesh*)meshModel->meshes->at(loop);
		

		GLuint diffuseNr = 1;
		GLuint specularNr = 1;

		if (processingMesh->textures.size() <= 0) {
			glUniform1i(difTexCountID, 0);
		}
		else {
			glUniform1i(difTexCountID, 1);

			for (GLuint i = 0; i < processingMesh->textures.size(); i++)
			{
				glActiveTexture(GL_TEXTURE0 + i); // Activate proper texture unit before binding
												  // Retrieve texture number (the N in diffuse_textureN)
				stringstream ss;
				string number;
				string name = processingMesh->textures[i].type;
				if (name == "texture_diffuse")
					ss << diffuseNr++; // Transfer GLuint to stream
				else if (name == "texture_specular")
					ss << specularNr++; // Transfer GLuint to stream		

										//cout << name << endl;

				number = ss.str();

				glBindTexture(GL_TEXTURE_2D, processingMesh->textures[i].id);
				glUniform1i(shader->GetUniformLocation((GLchar*)(name + number).c_str()), i);

				//std::cout << (name + number).c_str() << " / " << shader->GetUniformLocation(shader->GetID(), (name + number).c_str()) << endl;
			}
		}
		glActiveTexture(GL_TEXTURE0);

		// Draw mesh
		glBindVertexArray(processingMesh->VAO);

		glm::vec3 dirCam = glm::normalize(cam_->transform->position - transform->position);

		unsigned int eboIdx;
		
		glm::vec3 dirCamYZ = glm::normalize(glm::vec3(0, dirCam.y, dirCam.z));
		float angleVert = glm::dot(dirCamYZ, glm::vec3(0, 0, 1));
		if (dirCamYZ.y < 0)angleVert = -angleVert;
		angleVert = (-angleVert + 1) / 2 * samplingDirCount;//0 ~ sampleDirCount
		eboIdx = angleVert + 0.5f;
		eboIdx %= samplingDirCount;
		//eboIdx = 1;
		
		glm::vec3 planeNormal = processingMesh->sampleData[eboIdx].planeNormal;
		glm::vec3 dirCamOnPlane = dirCam - glm::dot(dirCam, planeNormal) * planeNormal;
		dirCamOnPlane = glm::normalize(dirCamOnPlane);
		float angleHori = glm::dot(dirCamOnPlane, processingMesh->sampleData[eboIdx].refVec);
		
		if (dirCam.x <= 0) {
			angleHori = -angleHori + 1;
		}
		else {
			angleHori += 3;
		}		//0 ~ 4로 전체 표현
		if (angleHori < 1)angleHori += 4;//val -> 1 ~ 5

		int mapperIdx = (arMapSize * angleHori / 6.0f) + 0.5f;//중간 기준
		int t = arMapSize / 6 + 0.5f + 6;
		if (mapperIdx - t < 0) {
			mapperIdx = t;
		}
		int startTriIdx = processingMesh->arMap[mapperIdx - t].idx; //시작
		
		if (mapperIdx + t > arMapSize) {
			mapperIdx = arMapSize - t;
		}
		int tCount = processingMesh->arMap[mapperIdx + t].idx - startTriIdx;
		/*cout << "angleH: " << angleHori << " / "
			<< "mapperIddx: " << mapperIdx << " / "
			<< "from: " << startTriIdx << " / "
			<< "count: " << tCount 
			<< endl;*/

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, processingMesh->EBOs[eboIdx]);
		glDrawElements(GL_TRIANGLES, tCount * 3, GL_UNSIGNED_INT, (GLvoid*)(startTriIdx * sizeof(Triangle)));
		glBindVertexArray(0);
	}

	glBindTexture(GL_TEXTURE_2D, 0);
}

