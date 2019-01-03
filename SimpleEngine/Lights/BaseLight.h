#pragma once

#include <glm\glm.hpp>
#include <string>

class BaseShader;

struct ShadowData {
	unsigned int resWidth = 2048;
	unsigned int resHeight = 2048;
		
	unsigned int depthMapTextureId;
	unsigned int depthMapFBO;
};

class BaseLight {
public:
	BaseLight();
	~BaseLight();

	float intensity;
	glm::vec3 color;
	glm::vec3 position;	

	std::string lightType;

	ShadowData shadowData;

	unsigned int modelMatrixId;
	glm::mat4 lightSpaceMatrix;

	bool isShadowCaster;
	
	virtual void InitShadowMap() = 0;
	virtual void EnableShadowMapBuffer() = 0;

protected:
	
};
