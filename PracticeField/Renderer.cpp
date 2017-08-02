#include "Renderer.h"


Renderer::Renderer(Transform* transform_){
	transform = transform_;
}


Renderer::~Renderer(){
	free(shader);
	free(transform);
}

void Renderer::SetShader(Shader* shader_){
	shader = shader_;
	mvpMatrixID = glGetUniformLocation(shader->GetID(), "MVP");
	viewMatrixID = glGetUniformLocation(shader->GetID(), "V");
	modelMatrixID = glGetUniformLocation(shader->GetID(), "M");
	lightPosID = glGetUniformLocation(shader->GetID(), "LightPosition_worldspace");
	lightColorID = glGetUniformLocation(shader->GetID(), "LightColor");
	lightPowerID = glGetUniformLocation(shader->GetID(), "LightPower");
}

void Renderer::Render(Camera * cam, std::vector<Light*> lights_, MeshModel* meshModel){
	ComputeModelMatrix(cam);

	glUseProgram(shader->GetID());

	glUniformMatrix4fv(mvpMatrixID, 1, GL_FALSE, glm::value_ptr(mvpMatrix));
	glUniformMatrix4fv(viewMatrixID, 1, GL_FALSE, glm::value_ptr(cam->Vmatrix()));
	glUniformMatrix4fv(modelMatrixID, 1, GL_FALSE, glm::value_ptr(modelMatrix));

	glUniform3f(lightPosID, lights_[0]->position.x, lights_[0]->position.y, lights_[0]->position.z);
	glUniform3f(lightColorID, lights_[0]->color.x, lights_[0]->color.y, lights_[0]->color.z);
	glUniform1f(lightPowerID, lights_[0]->intensity);


	for (GLuint loop = 0; loop < meshModel->meshes.size(); loop++) {
		Mesh* processingMesh = &meshModel->meshes[loop];

		GLuint diffuseNr = 1;
		GLuint specularNr = 1;
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
			glUniform1f(glGetUniformLocation(shader->GetID(), (name + number).c_str()), i);
		}
		glActiveTexture(GL_TEXTURE0);

		// Draw mesh
		glBindVertexArray(processingMesh->VAO);
		int s = processingMesh->triangles.size() * 3;

		//glm::dot(glm::vec3(1, 2, 3), glm::vec3(1, 2, 3));
		glm::vec3 dirCam = glm::normalize(cam->transform.position - transform->position);
		
		float dot = glm::dot(dirCam, glm::vec3(0, 0, -1));
		
		//glDrawElements(GL_TRIANGLES, s / 2, GL_UNSIGNED_INT, (GLvoid*)(s / 2 * sizeof(GLuint)));
		glDrawElements(GL_TRIANGLES, s, GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);
	}
}

void Renderer::ComputeModelMatrix(Camera * cam){
	modelMatrix = glm::translate(glm::mat4(1.0), transform->position);
	mvpMatrix = cam->VPmatrix() * modelMatrix;
}

