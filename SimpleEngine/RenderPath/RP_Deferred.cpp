#include "RP_Deferred.h"

#include "../Scenes/SceneIncludes.h"
#include <GameWindow.h>

#include "../Render/RenderData.h"
#include "../Render/Shaders/GBufferShader.h"

RP_Deferred::RP_Deferred() {
}


RP_Deferred::~RP_Deferred() {
}

void RP_Deferred::Initialize() {
	InitOffScreenDraw();
	
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

	gBufferShader = new GBufferShader();
}

void RP_Deferred::Render(Camera* mainCamera_, SceneRenderData* sceneRenderData_) {
	int rdrCount = sceneRenderData_->renderers.size();

	SceneRenderData_df* a = (SceneRenderData_df*)sceneRenderData_;
	

	mainCamera_->ComputeMatrix();
	for (int loop = 0; loop < rdrCount; loop++) {
		sceneRenderData_->renderers[loop]->ComputeMatrix(mainCamera_);
	}

	//Geometry Pass
	glBindFramebuffer(GL_FRAMEBUFFER, gBuffer);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	gBufferShader->Use();
	gBufferShader->SetMat4("P", mainCamera_->Pmatrix());
	gBufferShader->SetMat4("MVP", mainCamera_->Vmatrix());
	for (int loop = 0; loop < rdrCount; loop++) {
		gBufferShader->SetMat4("M", sceneRenderData_->renderers[loop]->Mmatrix());
		gBufferShader->SetMat4("M", sceneRenderData_->renderers[loop]->Mmatrix());
		//sceneRenderData_->renderers[loop]->Render();
	}
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	

	//Lighting Pass

	//Additional Forward
}
