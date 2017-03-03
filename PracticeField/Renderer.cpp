#include "Renderer.h"

Renderer::Renderer(Transform* transform_){
	objectTransform = transform_;	

	GLuint VertexArrayID;
	glGenVertexArrays(1, &VertexArrayID);
	glBindVertexArray(VertexArrayID);

	shaderID =
		LoadShaders(
			"Material/SimpleShader.shader"
			);

	mvpMatrixID = glGetUniformLocation(shaderID, "MVP");
	viewMatrixID = glGetUniformLocation(shaderID, "V");
	modelMatrixID = glGetUniformLocation(shaderID, "M");
	lightID = glGetUniformLocation(shaderID, "LightPosition_worldspace");

	texture = loadDDS("Materials/uvmap.DDS");
	textureID = glGetUniformLocation(shaderID, "myTextureSampler");

	loadOBJ("Materials/suzanne.obj", vertexData, uvData, normalData);

	glGenBuffers(1, &vertexBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, vertexData.size() * sizeof(glm::vec3), &vertexData[0], GL_STATIC_DRAW);

	glGenBuffers(1, &uvBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, uvBuffer);
	glBufferData(GL_ARRAY_BUFFER, uvData.size() * sizeof(glm::vec2), &uvData[0], GL_STATIC_DRAW);

	glGenBuffers(1, &normalBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, normalBuffer);
	glBufferData(GL_ARRAY_BUFFER, normalData.size() * sizeof(glm::vec3), &normalData[0], GL_STATIC_DRAW);

	
	/*
	glGenVertexArrays(1, &VertexArrayID);
	glBindVertexArray(VertexArrayID);*/
}

void Renderer::Render(Camera* cam){
	ComputeModelMatrix();

	glUseProgram(shaderID);		

	mvpMatrix = cam->VPmatrix() * modelMatrix;
	glUniformMatrix4fv(modelMatrixID, 1, GL_FALSE, &modelMatrix[0][0]);
	glUniformMatrix4fv(viewMatrixID, 1, GL_FALSE, &cam->Vmatrix()[0][0]);
	glUniformMatrix4fv(mvpMatrixID, 1, GL_FALSE, &mvpMatrix[0][0]);		

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture);
	// Set our "myTextureSampler" sampler to user Texture Unit 0
	glUniform1i(textureID, 0);

	glm::vec3 lightPos = glm::vec3(5, 5, 5);
	glUniform3f(lightID, lightPos.x, lightPos.y, lightPos.z);

	// 1rst attribute buffer : vertices
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
	glVertexAttribPointer(
		0,                  // attribute
		3,                  // size
		GL_FLOAT,           // type
		GL_FALSE,           // normalized?
		0,                  // stride
		(void*)0            // array buffer offset
		);

	// 2nd attribute buffer : UVs
	glEnableVertexAttribArray(1);
	glBindBuffer(GL_ARRAY_BUFFER, uvBuffer);
	glVertexAttribPointer(
		1,                                // attribute
		2,                                // size
		GL_FLOAT,                         // type
		GL_FALSE,                         // normalized?
		0,                                // stride
		(void*)0                          // array buffer offset
		);

	glEnableVertexAttribArray(2);
	glBindBuffer(GL_ARRAY_BUFFER, normalBuffer);
	glVertexAttribPointer(
		2,                                // attribute
		3,                                // size
		GL_FLOAT,                         // type
		GL_FALSE,                         // normalized?
		0,                                // stride
		(void*)0                          // array buffer offset
		);

	// Draw the triangle !
	glDrawArrays(GL_TRIANGLES, 0, vertexData.size());

	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
	glDisableVertexAttribArray(2);
}

void Renderer::ComputeModelMatrix() {
	modelMatrix = glm::translate(modelMatrix, objectTransform->position);
	//printf("%f\n", objectTransform->position.x);
}


Renderer::~Renderer()
{
}
