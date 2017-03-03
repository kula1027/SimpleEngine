#pragma once

#include <iostream>
#include <glm\glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "MaterialLoader.h"
#include "Camera.h"

class Renderer
{
private:
	GLuint texture;
	GLuint textureID;
	GLuint mvpMatrixID;
	GLuint viewMatrixID;
	GLuint modelMatrixID;
	GLuint lightID;

	ShaderMaterial* shaderMaterial;

	glm::mat4 modelMatrix;

	std::vector<glm::vec3> vertexData;
	GLuint vertexBuffer;

	std::vector<glm::vec2> uvData;
	GLuint uvBuffer;

	std::vector<glm::vec3> normalData;
	GLuint normalBuffer;

	Transform* objectTransform;

	void ComputeModelMatrix();
	glm::mat4 mvpMatrix;
public:	
	Renderer(Transform* transform);
	~Renderer();

	void Render(Camera* cam);
	//void SetObj();
};

