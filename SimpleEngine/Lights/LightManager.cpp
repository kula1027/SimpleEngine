#include "LightManager.h"
#include "BaseLight.h"
#include <Shaders/BaseShader.h>
#include <gl/glew.h>
#include <EngineDef.h>

LightManager* LightManager::instance;

LightManager::LightManager() {
	glGenBuffers(1, &uboLightData);
	glBindBuffer(GL_UNIFORM_BUFFER, uboLightData);
	glBufferData(GL_UNIFORM_BUFFER, 26644, NULL, GL_STATIC_DRAW);

	glBindBufferBase(GL_UNIFORM_BUFFER, BindingPointLightData, uboLightData);
	// or glBindBufferRange(GL_UNIFORM_BUFFER, 2, uboBlock, 0, 152);	

	glm::vec3 red = glm::vec3(0.5, 0.5, 0.5);
	glm::vec3 blue = glm::vec3(0, 0, 1);
	glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(glm::vec4), &red);
	glBufferSubData(GL_UNIFORM_BUFFER, sizeof(glm::vec4), sizeof(glm::vec4), &blue);

	SetAmbient(glm::vec3(0.1, 0.1, 0.1));
}
	

LightManager * LightManager::Inst() {
	if (instance == NULL) {
		instance = new LightManager();
	}

	return instance;
}

void LightManager::SetAmbient(glm::vec3 ambient_) {
	ambient = ambient_;

	glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(glm::vec4), &ambient);
}

void LightManager::AddLight(BaseLight* light_) {
	lights.push_back(light_);
	
	int lightCount = lights.size();
	glBufferSubData(GL_UNIFORM_BUFFER, 26640, sizeof(int), &lightCount);

	for (int loop = 0; loop < lightCount; loop++) {
		lights[loop]->SetUniforms_ubo(16 + loop * 52);
	}
}