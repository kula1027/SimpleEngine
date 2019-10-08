#include "RP_Deferred.h"

#include "../Scenes/SceneIncludes.h"
#include <GameWindow.h>

#include <Shaders/Deferred/ShaderDeferredGeo.h>
#include <Shaders/Deferred/ShaderDeferredLight.h>
#include <Shaders/ShaderManager.h>
#include <Lights/LightManager.h>

RP_Deferred::RP_Deferred() {
}


RP_Deferred::~RP_Deferred() {
}

void RP_Deferred::Initialize() {
	InitOffScreenDraw();

	glEnable(GL_DEPTH_TEST);	
	
	glGenFramebuffers(1, &gBuffer);
	glBindFramebuffer(GL_FRAMEBUFFER, gBuffer);	
	
	//position buffer
	glGenTextures(1, &gPosition);
	glBindTexture(GL_TEXTURE_2D, gPosition);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, GameWindow::GetWidth(), GameWindow::GetHeight(), 0, GL_RGB, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, gPosition, 0);//texture를 fb에 attach한다.

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

	glDrawBuffers(3, attachments);//Draw할 Attachements를 지정

	unsigned int rboDepth;
	glGenRenderbuffers(1, &rboDepth);
	glBindRenderbuffer(GL_RENDERBUFFER, rboDepth);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, GameWindow::GetWidth(), GameWindow::GetHeight());
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, rboDepth);
	// finally check if framebuffer is complete
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		std::cout << "Framebuffer not complete!" << std::endl;	

	shaderDeferredGeo = new ShaderDeferredGeo();
	shaderDeferredLight = new ShaderDeferredLight();

	shaderDeferredLight->Use();
	shaderDeferredLight->SetInt("gPosition", 0);
	shaderDeferredLight->SetInt("gNormal", 1);
	shaderDeferredLight->SetInt("gAlbedoSpec", 2);

}

void RP_Deferred::Render(Camera* mainCamera_, SceneRenderData* sceneRenderData_) {
	int rdrCount_Deferred = sceneRenderData_->renderQueue_Deferred.size();
	int rdrCount_Forward = sceneRenderData_->renderQueue_Forward.size();	
	
	mainCamera_->SetUpMatrices();
	for (int loop = 0; loop < rdrCount_Deferred; loop++) {
		sceneRenderData_->renderQueue_Deferred[loop]->ComputeMatrices(mainCamera_);
	}
	for (int loop = 0; loop < rdrCount_Forward; loop++) {
		sceneRenderData_->renderQueue_Forward[loop]->ComputeMatrices(mainCamera_);
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
	glBindFramebuffer(GL_FRAMEBUFFER, offScreenData.frameBuffer);
	glClearColor(0, 0.3f, 0.3f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


	//LightPass Ambient
	shaderDeferredLight->Use();
	glActiveTexture(GL_TEXTURE0);//다음 bind tex는 GL_TEXTURE0에 대해서.
	glBindTexture(GL_TEXTURE_2D, gPosition);//
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, gNormal);
	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, gAlbedoSpec);
	shaderDeferredLight->SetVec3("viewPos", mainCamera_->transform->position);
	glBindVertexArray(offScreenData.quadVAO);
	glDrawArrays(GL_TRIANGLES, 0, 6);	
	
	for (int loop = 0; loop < LightManager::Inst()->pointLights.size(); loop++) {
		//TODO
	}
	
	//For Each Lights
		//switch LightType
		//use Light Shader
		//Render Light
		//Blend Light

	

	//Additional Forward
	glBindFramebuffer(GL_READ_FRAMEBUFFER, gBuffer);//gBuffer에서 읽어서
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, offScreenData.frameBuffer);	//default buffer에 그린다
	glBlitFramebuffer(//무엇을? 깊이 버퍼 값을
		0, 0, GameWindow::GetWidth(), GameWindow::GetHeight(),
		0, 0, GameWindow::GetWidth(), GameWindow::GetHeight(),
		GL_DEPTH_BUFFER_BIT, GL_NEAREST);
	for (int loop = 0; loop < rdrCount_Forward; loop++) {
		sceneRenderData_->renderQueue_Forward[loop]->RenderMesh_Forward(mainCamera_);
	}	


	//Render off rendered frame to default fb
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glDisable(GL_DEPTH_TEST);
	glClearColor(0.0f, 0.7f, 0.2f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);
	offScreenData.screenShader->Use();
	glBindVertexArray(offScreenData.quadVAO);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, offScreenData.texColorBuffer);
	glDrawArrays(GL_TRIANGLES, 0, 6);

	glEnable(GL_DEPTH_TEST);
}
