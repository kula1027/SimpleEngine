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

	SetAmbient(glm::vec3(0.3, 0.3, 0.3));
}
	

LightManager * LightManager::Inst() {
	if (instance == NULL) {
		instance = new LightManager();
	}

	return instance;
}

void LightManager::SetAmbient(glm::vec3 ambient_) {
	ambient = ambient_;

	glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(glm::vec3), &ambient);
}

void LightManager::AddLight(BaseLight* light_) {
	lights.push_back(light_);
	

	glBindBuffer(GL_UNIFORM_BUFFER, uboLightData);

	int lightCount = lights.size();
	glBufferSubData(GL_UNIFORM_BUFFER, 26640, 4, &lightCount);

	int addIdx = lightCount - 1;	
	lights[addIdx]->SetUniforms_ubo(16 + addIdx * 52);

	//lights[addIdx]->SetUniforms_ubo(16 + 52);
}