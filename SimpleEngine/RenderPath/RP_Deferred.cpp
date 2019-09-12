#include "RP_Deferred.h"

#include "../Scenes/SceneIncludes.h"
#include <GameWindow.h>

#include <Shaders/Deferred/ShaderDeferredGeo.h>
#include <Shaders/Deferred/ShaderDeferredLight.h>

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
	glBindFramebuffer(GL_FRAMEBUFFER, 0);


	shaderDeferredGeo = new ShaderDeferredGeo();
	shaderDeferredLight = new ShaderDeferredLight();

	shaderDeferredLight->Use();
	shaderDeferredLight->SetInt("gPosition", 0);
	shaderDeferredLight->SetInt("gNormal", 1);
	shaderDeferredLight->SetInt("gAlbedoSpec", 2);

}

void RP_Deferred::Render(Camera* mainCamera_, SceneRenderData* sceneRenderData_) {
	int rdrCount = sceneRenderData_->renderers.size();
	int lightCount = sceneRenderData_->lights.size();
	
	mainCamera_->ComputeMatrix();
	for (int loop = 0; loop < rdrCount; loop++) {
		sceneRenderData_->renderers[loop]->ComputeMatrix(mainCamera_);
	}

	//Geometry Pass
	glBindFramebuffer(GL_FRAMEBUFFER, gBuffer);	
	glClearColor(0.0f, 1.0f, 1.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	shaderDeferredGeo->Use();
	shaderDeferredGeo->SetMat4("VP", mainCamera_->VPmatrix());
	for (int loop = 0; loop < rdrCount; loop++) {
		shaderDeferredGeo->SetMat4("M", sceneRenderData_->renderers[loop]->Mmatrix());
		sceneRenderData_->renderers[loop]->RenderMesh();
	}

	//Lighting Pass
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glClearColor(1.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	shaderDeferredLight->Use();

	glActiveTexture(GL_TEXTURE0);//���� bind tex�� GL_TEXTURE0�� ���ؼ�.
	glBindTexture(GL_TEXTURE_2D, gPosition);//
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, gNormal);
	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, gAlbedoSpec);
	for (int loop = 0; loop < lightCount; loop++) {
		shaderDeferredLight->SetLightUniforms(sceneRenderData_->lights.at(loop), loop);
	}
	shaderDeferredLight->SetVec3("viewPos", mainCamera_->transform->position);
	
	glBindFramebuffer(GL_READ_FRAMEBUFFER, gBuffer);//gBuffer���� �о
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);	//default buffer�� �׸���
	glBlitFramebuffer(//������? ���� ���� ����
		0, 0, GameWindow::GetWidth(), GameWindow::GetHeight(), 
		0, 0, GameWindow::GetWidth(), GameWindow::GetHeight(), 
		GL_DEPTH_BUFFER_BIT, GL_NEAREST);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	glBindVertexArray(offScreenData.quadVAO);
	glDrawArrays(GL_TRIANGLES, 0, 6);

	//Additional Forward
}
