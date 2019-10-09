#include "LightManager.h"
#include "BaseLight.h"
#include "LightsBundle.h"

#include <Shaders/BaseShader.h>
#include <gl/glew.h>
#include <EngineDef.h>


LightManager* LightManager::instance;

LightManager::LightManager() {
	glGenBuffers(1, &uboLightData);
	glBindBuffer(GL_UNIFORM_BUFFER, uboLightData);
	glBufferData(GL_UNIFORM_BUFFER, 16384, NULL, GL_STATIC_DRAW);

	glBindBufferBase(GL_UNIFORM_BUFFER, BindingPointLightData, uboLightData);
	// or glBindBufferRange(GL_UNIFORM_BUFFER, 2, uboBlock, 0, 152);	

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

	//16 - 32
	glBufferSubData(GL_UNIFORM_BUFFER, sizeof(glm::vec4), sizeof(glm::vec3), &ambient);
}

void LightManager::AddLight(BaseLight* light_) {
	lights.push_back(light_);		

	glBindBuffer(GL_UNIFORM_BUFFER, uboLightData);	
	int startAddr;
	int lightCount;
	switch (light_->GetLightType()) {
	case LightType_Directional:	
		lightCount = directionalLights.size();
		startAddr = 32 + 32 * directionalLights.size();		
		directionalLights.insert(make_pair(light_->GetComponentId(), dynamic_cast<DirectionalLight*>(light_)));								
		glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(int), &(++lightCount));
		break;

	case LightType_Point:
		lightCount = pointLights.size();
		startAddr = 544 + 32 * lightCount;
		pointLights.insert(make_pair(light_->GetComponentId(), dynamic_cast<PointLight*>(light_)));		
		glBufferSubData(GL_UNIFORM_BUFFER, sizeof(int), sizeof(int), &(++lightCount));
		break;

	case LightType_Spot:

		break;

	default:		
		break;
	}	

	light_->SetStartAddrUbo(startAddr);
	light_->SetUniformsUbo();
}