#pragma once
#include "GameObject.h"
#include <gl\glew.h>
#include <glm\glm.hpp>
#include <glm\gtc\matrix_transform.hpp>


class Transform;
class BaseShader;
class RenderPath;
class SceneRenderData;

#define PROJECTION_ORTHO 0
#define PROJECTION_PERSPECTIVE 1

class Camera : public GameObject{
private:
	float fov;
	float near;
	float far;

	RenderPath* renderPath;

	glm::vec3 upVector;
	
	glm::mat4 viewMatrix;
	glm::mat4 projectionMatrix;
	glm::mat4 vpMatrix;

	int projMode;

public:
	Camera();
	~Camera();
	glm::mat4 VPmatrix();
	glm::mat4 Vmatrix();
	glm::mat4 Pmatrix();

	glm::vec4 clearColor;
	glm::vec2 normalizedViewPort;

	SkyBox * skybox;

	virtual void Initialize();

	void Render(SceneRenderData* renderData_);
	int renderMode;

	void ComputeMatrix();
	void RenderSkyBox();
};

