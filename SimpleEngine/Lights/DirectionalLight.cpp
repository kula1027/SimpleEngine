#include "DirectionalLight.h"
#include <gl\glew.h>
#include <glm\gtc\matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <FilePooler.h>
#include <Shaders/BaseShader.h>
#include <Bases/Transform.h>
#include <Lights/LightManager.h>
#include <Scene/SceneRenderData.h>
#include <Render/MeshRenderer.h>

DirectionalLight::DirectionalLight(){	
	color = glm::vec3(0.1, 0.1, 0.1);	
	castShadow = true;
	lightType = LightType_Directional;	
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

	shadowMapShader = new BaseShader("Shadow/shadowDepth");

	lightVPId = shadowMapShader->GetUniformLocation("lightSpaceMatrix");		
}

void DirectionalLight::SetUbo_Intensity() {

}

void DirectionalLight::RenderShadowMap(SceneRenderData* srd_){
	lightProjection = glm::ortho(-50.0f, 50.0f, -50.0f, 50.0f, near_plane, far_plane);

	lightView = glm::lookAt(
		glm::vec3(-5.0f, 15.0f, 0.0f),
		glm::vec3(-5.0f, 15.0f, 0.0f) + GetTransform()->GetForward(),
		glm::vec3(0.0f, 1.0f, 0.0f));
	lightVP = lightProjection * lightView;

	shadowMapShader->Use();
	
	glUniformMatrix4fv(lightVPId, 1, GL_FALSE, glm::value_ptr(lightVP));

	glViewport(0, 0, shadowMapData.resWidth, shadowMapData.resHeight);
	glBindFramebuffer(GL_FRAMEBUFFER, shadowMapData.depthMapFBO);	
	glClear(GL_DEPTH_BUFFER_BIT);

	int rdrCount_Deferred = srd_->renderQueue_Deferred.size();
	for (int loop = 0; loop < rdrCount_Deferred; loop++) {		
		shadowMapShader->SetMat_M(srd_->renderQueue_Deferred[loop]->Mmatrix());
		srd_->renderQueue_Deferred[loop]->RenderMesh_NoTexture();
	}
	int rdrCount_Forward = srd_->renderQueue_Forward.size();
	for (int loop = 0; loop < rdrCount_Forward; loop++) {
		shadowMapShader->SetMat_M(srd_->renderQueue_Forward[loop]->Mmatrix());
		srd_->renderQueue_Forward[loop]->RenderMesh_NoTexture();
	}
}

ShadowMapData DirectionalLight::GetShadowMapData() {
	return shadowMapData;
}

void DirectionalLight::BindShadowMap() {
	glActiveTexture(GL_TEXTURE3);
	glBindTexture(GL_TEXTURE_2D, shadowMapData.depthMapTextureId);
}

void DirectionalLight::SetUbo() {
	LightManager::Inst()->BindUboLightData(); 

	//direction 0 - 16
	vec4 lightDir = vec4(GetTransform()->GetForward(), 0);
	glBufferSubData(GL_UNIFORM_BUFFER, 
		startAddrUbo,
		sizeof(glm::vec4), 
		glm::value_ptr(lightDir));
	//color 16 - 32
	glBufferSubData(GL_UNIFORM_BUFFER, 
		startAddrUbo + sizeof(glm::vec4),
		sizeof(glm::vec4),
		&color);
	
	//lightVP 32 - 96
	glBufferSubData(GL_UNIFORM_BUFFER,
		startAddrUbo + sizeof(glm::vec4) * 2,
		sizeof(glm::mat4),
		&lightVP);
}

void DirectionalLight::OnTransformChanged() {	
	lightView = glm::lookAt(
		glm::vec3(-5.0f, 15.0f, 0.0f),
		glm::vec3(-5.0f, 15.0f, 0.0f) + GetTransform()->GetForward(),
		glm::vec3(0.0f, 1.0f, 0.0f));
	lightVP = lightProjection * lightView;

	SetUbo();
}

void DirectionalLight::OnAttachedToObject(EngineObject * obj_) {
	BaseLight::OnAttachedToObject(obj_);

	lightProjection = glm::ortho(-50.0f, 50.0f, -50.0f, 50.0f, near_plane, far_plane);
	lightView = glm::lookAt(
		glm::vec3(-5.0f, 15.0f, 0.0f),
		glm::vec3(-5.0f, 15.0f, 0.0f) + GetTransform()->GetForward(),
		glm::vec3(0.0f, 1.0f, 0.0f));
	lightVP = lightProjection * lightView;

	InitShadowMap();
}
