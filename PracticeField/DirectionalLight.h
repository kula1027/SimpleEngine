#pragma once

#include "BaseLight.h"

class DirectionalLight : public BaseLight{
private:
	Shader* shadowMapShader;

	unsigned int lightSpaceMatrixId;
	glm::mat4 lightView;
	
	glm::mat4 lightProjection;
	void InitShadowMap();

public:
	DirectionalLight();
	~DirectionalLight();


	void EnableShadowMapBuffer();

	float near_plane = 1.0f, far_plane = 1000.0f;

	
};