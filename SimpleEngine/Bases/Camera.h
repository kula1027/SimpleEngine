#pragma once
#include "EngineObject.h"
#include <gl\glew.h>
#include <glm\glm.hpp>
#include <glm\gtc\matrix_transform.hpp>


class Transform;
class BaseShader;
class RenderPath;
class SceneRenderData;

#define PROJECTION_ORTHO 0
#define PROJECTION_PERSPECTIVE 1

class Camera : public EngineObject{
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
	int renderMode;

	SkyBox * skybox;

public:
	Camera();
	~Camera();
	glm::mat4 VPmatrix();
	glm::mat4 Vmatrix();
	glm::mat4 Pmatrix();

	glm::vec4 clearColor;
	glm::vec2 normalizedViewPort;

	void SetSkybox(SkyBox* skybox_);
	SkyBox* GetSkybox();

	void Render(SceneRenderData* renderData_);

	void ComputeMatrix();
	void RenderSkyBox();
};

