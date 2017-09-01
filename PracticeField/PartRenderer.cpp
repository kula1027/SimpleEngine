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

		glm::vec3 dirCam = cam_->transform->position - transform->position;
		dirCam.y = 0;
		dirCam = glm::normalize(dirCam);

		float angleXZ = glm::dot(dirCam, glm::vec3(0, 0, 1));
		if (dirCam.x <= 0) {
			angleXZ = -angleXZ + 1;
		}
		else {
			angleXZ += 3;
		}		//0 ~ 4로 전체 표현
		if (angleXZ < 1)angleXZ += 4;//val -> 0 ~ 5

		int mapperIdx = (arMapSize * angleXZ / 6.0f);//중간 기준
		int startTriIdx = processingMesh->arMap[mapperIdx - arMapSize/6].idx; //시작
		int tCount = processingMesh->arMap[mapperIdx + arMapSize/6].idx - startTriIdx;

		glDrawElements(GL_TRIANGLES, tCount * 3, GL_UNSIGNED_INT, (GLvoid*)(startTriIdx * sizeof(Triangle)));
		glBindVertexArray(0);
	}

	glBindTexture(GL_TEXTURE_2D, 0);
}

