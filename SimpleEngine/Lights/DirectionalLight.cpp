#include "DirectionalLight.h"
#include <gl\glew.h>
#include <glm\gtc\matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "../FilePooler.h"
#include <Shaders/BaseShader.h>
#include <Bases/Transform.h>


DirectionalLight::DirectionalLight(){
	intensity = 0.8f;
	color = glm::vec3(1, 0, 1);	
	isShadowCaster = true;

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
	glGenTextures(1, &shadowData.depthMapTextureId);
	glGenFramebuffers(1, &shadowData.depthMapFBO);

	glBindTexture(GL_TEXTURE_2D, shadowData.depthMapTextureId);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT,
		shadowData.resWidth, shadowData.resHeight, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);	
	float borderColor[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);
//	glBindTexture(GL_TEXTURE_2D, 0);

	glBindFramebuffer(GL_FRAMEBUFFER, shadowData.depthMapFBO);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, shadowData.depthMapTextureId, 0);
	glDrawBuffer(GL_NONE);
	glReadBuffer(GL_NONE);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	shadowMapShader = new BaseShader("shadowMap");
	lightSpaceMatrixId = shadowMapShader->GetUniformLocation("lightSpaceMatrix");
	modelMatrixId = shadowMapShader->GetUniformLocation("modelMatrix");
}

void DirectionalLight::EnableShadowMapBuffer(){
	lightProjection = glm::ortho(-30.0f, 30.0f, -30.0f, 30.0f, near_plane, far_plane);

	lightView = glm::lookAt(-GetTransform()->position,
		glm::vec3(0.0f, 0.0f, 0.0f),
		glm::vec3(0.0f, 1.0f, 0.0f));

	shadowMapShader->Use();
	lightSpaceMatrix = lightProjection * lightView;
	glUniformMatrix4fv(lightSpaceMatrixId, 1, GL_FALSE, glm::value_ptr(lightSpaceMatrix));

	glViewport(0, 0, shadowData.resWidth, shadowData.resHeight);
	glBindFramebuffer(GL_FRAMEBUFFER, shadowData.depthMapFBO);

	glEnable(GL_DEPTH_TEST);
	glClear(GL_DEPTH_BUFFER_BIT);
}

void DirectionalLight::SetUniforms_ubo(int startAddr_) {
	glBufferSubData(GL_UNIFORM_BUFFER, startAddr_ + sizeof(glm::vec4), sizeof(glm::vec4), &GetTransform()->GetForward());//direction
	glBufferSubData(GL_UNIFORM_BUFFER, startAddr_ + sizeof(glm::vec4) * 2, sizeof(glm::vec4), &color);//color
	glBufferSubData(GL_UNIFORM_BUFFER, startAddr_ + sizeof(glm::vec4) * 3, sizeof(int), &lightType);//lightType
}
