#include "DirectionalLight.h"
#include <gl\glew.h>
#include <glm\gtc\matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <FilePooler.h>
#include <Shaders/BaseShader.h>
#include <Bases/Transform.h>
#include <Lights/LightManager.h>

DirectionalLight::DirectionalLight(){	
	color = glm::vec3(0.5, 0.5, 0.5);	
	castShadow = true;
	lightType = LightType_Directional;

	glm::mat4 lightProjection = glm::ortho(-100.0f, 100.0f, -100.0f, 100.0f, 0.1f, 100.0f);
	/*glm::mat4 lightView = glm::lookAt(
		GetTransform()->position,
		glm::vec3(0.0f, 0.0f, 0.0f),
		glm::vec3(0.0f, 1.0f, 0.0f));*/
	glm::mat4 lightSpaceMatrix = lightProjection * lightView;

	InitShadowMap();
}


DirectionalLight::~DirectionalLight(){

}

void DirectionalLight::InitShadowMap(){
	glGenTextures(1, &shadowMapData.depthMapTextureId);
	glGenFramebuffers(1, &shadowMapData.depthMapFBO);

	glBindTexture(GL_TEXTURE_2D, shadowMapData.depthMapTextureId);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT,
		shadowMapData.resWidth, shadowMapData.resHeight, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);	
	float borderColor[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);

	glBindFramebuffer(GL_FRAMEBUFFER, shadowMapData.depthMapFBO);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, shadowMapData.depthMapTextureId, 0);
	glDrawBuffer(GL_NONE);
	glReadBuffer(GL_NONE);

	shadowMapShader = new BaseShader("shadowMap");
	lightSpaceMatrixId = shadowMapShader->GetUniformLocation("lightSpaceMatrix");
	modelMatrixId = shadowMapShader->GetUniformLocation("modelMatrix");
}

void DirectionalLight::SetUbo_Intensity() {

}

void DirectionalLight::EnableShadowMapBuffer(){
	lightProjection = glm::ortho(-30.0f, 30.0f, -30.0f, 30.0f, near_plane, far_plane);

	lightView = glm::lookAt(
		-GetTransform()->GetForward(),
		glm::vec3(0.0f, 0.0f, 0.0f),
		glm::vec3(0.0f, 1.0f, 0.0f));

	shadowMapShader->Use();
	lightSpaceMatrix = lightProjection * lightView;
	glUniformMatrix4fv(lightSpaceMatrixId, 1, GL_FALSE, glm::value_ptr(lightSpaceMatrix));

	glViewport(0, 0, shadowMapData.resWidth, shadowMapData.resHeight);
	glBindFramebuffer(GL_FRAMEBUFFER, shadowMapData.depthMapFBO);

	glEnable(GL_DEPTH_TEST);
	glClear(GL_DEPTH_BUFFER_BIT);
}

ShadowMapData DirectionalLight::GetShadowMapData() {
	return shadowMapData;
}

void DirectionalLight::SetUbo() {
	LightManager::Inst()->BindUboLightData(); 

	//direction 0 - 16
	vec4 lightDir = vec4(GetTransform()->GetForward(), 0);
	glBufferSubData(GL_UNIFORM_BUFFER, 
		startAddrUbo,
		sizeof(glm::vec4), 
		glm::value_ptr(lightDir));
	//color 16-32
	glBufferSubData(GL_UNIFORM_BUFFER, 
		startAddrUbo + sizeof(glm::vec4),
		sizeof(glm::vec4),
		&color);
	
}

void DirectionalLight::OnTransformChanged() {
	SetUbo();
}
