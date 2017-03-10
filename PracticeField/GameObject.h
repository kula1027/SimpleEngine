#pragma once

#include <vector>
#include <glm\glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "MaterialLoader.h"
#include "IUpdatable.h"
#include "Light.h"
#include "Transform.h"
#include "Camera.h"

class GameObject : public IUpdatable{
private:
	GLuint mvpMatrixID;
	GLuint viewMatrixID;
	GLuint modelMatrixID;
	GLuint lightPosID;
	GLuint lightColorID;
	GLuint lightPowerID;

	Shader* shader;
	MeshModel* meshModel;

	glm::mat4 modelMatrix;
	glm::mat4 mvpMatrix;
	void ComputeModelMatrix(Camera* cam);

public:
	GameObject();
	void Render(Camera* cam, std::vector<Light*> lights_);
	Transform transform;

	void SetModel(MeshModel* meshModel_);
	void SetShader(Shader* shader_);

	void Update();
	~GameObject();
};