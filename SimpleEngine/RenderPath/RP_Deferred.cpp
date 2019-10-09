#include "RP_Deferred.h"

#include "../Scenes/SceneIncludes.h"
#include <GameWindow.h>

#include <Shaders/Deferred/ShaderDeferredGeo.h>
#include <Shaders/Deferred/ShaderDeferredLight.h>
#include <Shaders/ShaderManager.h>
#include <Lights/LightManager.h>

void RP_Deferred::SetupFrameBuffers() {

	glGenFramebuffers(1, &gBuffer);
	glBindFramebuffer(GL_FRAMEBUFFER, gBuffer);

	//position buffer
	glGenTextures(1, &gPosition);
	glBindTexture(GL_TEXTURE_2D, gPosition);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, GameWindow::GetWidth(), GameWindow::GetHeight(), 0, GL_RGB, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, gPosition, 0);//texture�� fb�� attach�Ѵ�.

	//normal buffer
	glGenTextures(1, &gNormal);
	glBindTexture(GL_TEXTURE_2D, gNormal);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, GameWindow::GetWidth(), GameWindow::GetHeight(), 0, GL_RGB, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D, gNormal, 0);

	//color + specular buffer
	glGenTextures(1, &gAlbedoSpec);
	glBindTexture(GL_TEXTURE_2D, gAlbedoSpec);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, GameWindow::GetWidth(), GameWindow::GetHeight(), 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT2, GL_TEXTURE_2D, gAlbedoSpec, 0);

	attachments[0] = GL_COLOR_ATTACHMENT0;
	attachments[1] = GL_COLOR_ATTACHMENT1;
	attachments[2] = GL_COLOR_ATTACHMENT2;

	glDrawBuffers(3, attachments);//Draw�� Attachements�� ����

	unsigned int rboDepth;
	glGenRenderbuffers(1, &rboDepth);
	glBindRenderbuffer(GL_RENDERBUFFER, rboDepth);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, GameWindow::GetWidth(), GameWindow::GetHeight());
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, rboDepth);
	// finally check if framebuffer is complete
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		std::cout << "Framebuffer not complete!" << std::endl;
}

void RP_Deferred::SetupShaders() {
	//Shader Setup
	shaderDeferredGeo = new ShaderDeferredGeo();
	//shaderDeferredAmbient = new ShaderDeferredLight();
	shaderDeferredAmbient = new BaseShader("Deferred/deferred_light_ambient");
	shaderDeferredDirectional = new BaseShader("Deferred/deferred_light_directional");
	shaderDeferredPoint = new BaseShader("Deferred/deferred_light_point");

	shaderDeferredAmbient->Use();
	shaderDeferredAmbient->BindLightUBO();
	shaderDeferredAmbient->SetInt("gPosition", 0);
	shaderDeferredAmbient->SetInt("gNormal", 1);
	shaderDeferredAmbient->SetInt("gAlbedoSpec", 2);

	shaderDeferredDirectional->Use();
	shaderDeferredDirectional->BindLightUBO();
	shaderDeferredDirectional->SetInt("gPosition", 0);
	shaderDeferredDirectional->SetInt("gNormal", 1);
	shaderDeferredDirectional->SetInt("gAlbedoSpec", 2);
}

void RP_Deferred::ProcessLightPass() {
	glBindFramebuffer(GL_FRAMEBUFFER, offScreenData.frameBuffer);
	glClearColor(0, 0.3f, 0.3f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	
	shaderDeferredDirectional->Use();
	glActiveTexture(GL_TEXTURE0);//���� bind tex�� GL_TEXTURE0�� ���ؼ�.
	glBindTexture(GL_TEXTURE_2D, gPosition);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, gNormal);
	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, gAlbedoSpec);
	shaderDeferredDirectional->SetVec3("viewPos", targetCamera->transform->position);
	DrawOffScreenQuad();

	//LightPass Directional
	//glDisable(GL_DEPTH_TEST);
	//glEnable(GL_BLEND);
	//glBlendEquation(GL_FUNC_ADD);
	//glBlendFunc(GL_ONE, GL_ONE);	
	//
	//glDisable(GL_BLEND);
	//glEnable(GL_DEPTH_TEST);

	//LightPass Point
	for (int loop = 0; loop < LightManager::Inst()->pointLights.size(); loop++) {
		//TODO
	}
}

RP_Deferred::RP_Deferred() {
	InitOffScreenDraw();

	SetupFrameBuffers();

	SetupShaders();
}


RP_Deferred::~RP_Deferred() {
}

void RP_Deferred::Render(SceneRenderData* sceneRenderData_) {
	int rdrCount_Deferred = sceneRenderData_->renderQueue_Deferred.size();
	int rdrCount_Forward = sceneRenderData_->renderQueue_Forward.size();	
	
	targetCamera->SetUpMatrices();
	for (int loop = 0; loop < rdrCount_Deferred; loop++) {
		sceneRenderData_->renderQueue_Deferred[loop]->ComputeMatrices(targetCamera);
	}
	for (int loop = 0; loop < rdrCount_Forward; loop++) {
		sceneRenderData_->renderQueue_Forward[loop]->ComputeMatrices(targetCamera);
	}

	//Geometry Pass		
	glBindFramebuffer(GL_FRAMEBUFFER, gBuffer);	
	glClearColor(0.3f, 0.3f, 0.3f, 1.0f);	
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	shaderDeferredGeo->Use();	
	for (int loop = 0; loop < rdrCount_Deferred; loop++) {
		shaderDeferredGeo->SetModelMat(sceneRenderData_->renderQueue_Deferred[loop]->Mmatrix());
		sceneRenderData_->renderQueue_Deferred[loop]->RenderMesh();
	}	
	//mainCamera_->RenderSkyBox();

	//Lighting Pass
	ProcessLightPass();

	//Additional Forward
	glBindFramebuffer(GL_READ_FRAMEBUFFER, gBuffer);//gBuffer���� �о
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, offScreenData.frameBuffer);	//default buffer�� �׸���
	glBlitFramebuffer(//������? ���� ���� ����
		0, 0, GameWindow::GetWidth(), GameWindow::GetHeight(),
		0, 0, GameWindow::GetWidth(), GameWindow::GetHeight(),
		GL_DEPTH_BUFFER_BIT, GL_NEAREST);
	for (int loop = 0; loop < rdrCount_Forward; loop++) {
		sceneRenderData_->renderQueue_Forward[loop]->RenderMesh_Forward(targetCamera);
	}	


	//Render off rendered frame to default fb
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glDisable(GL_DEPTH_TEST);
	glClearColor(0.0f, 0.7f, 0.2f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);
	offScreenData.screenShader->Use();
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, offScreenData.texColorBuffer);
	DrawOffScreenQuad();

	glEnable(GL_DEPTH_TEST);
}
