#include "RP_Forward.h"
#include "../Scenes/SceneIncludes.h"
#include "../GameWindow.h"


RP_Forward::RP_Forward() {
	
}

RP_Forward::~RP_Forward() {
}

void RP_Forward::InitOffScreenDraw() {
	offScreenData.screenShader = FilePooler::LoadShader("PostProcess/defaultScreen.vert", "PostProcess/defaultScreen.frag");
	offScreenData.screenShader->Use();
	glUniform1i(offScreenData.screenShader->GetUniformLocation("screenTexture"), 0);

	glGenVertexArrays(1, &offScreenData.quadVAO);
	glGenBuffers(1, &offScreenData.quadVBO);
	glBindVertexArray(offScreenData.quadVAO);
	glBindBuffer(GL_ARRAY_BUFFER, offScreenData.quadVBO);
	glBufferData(GL_ARRAY_BUFFER, quadSize * sizeof(float), &quadVertices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));

	glGenFramebuffers(1, &offScreenData.frameBuffer);
	glBindFramebuffer(GL_FRAMEBUFFER, offScreenData.frameBuffer);

	glGenTextures(1, &offScreenData.texColorBuffer);
	glBindTexture(GL_TEXTURE_2D, offScreenData.texColorBuffer);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, GameWindow::GetWidth(), GameWindow::GetHeight(), 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glBindTexture(GL_TEXTURE_2D, 0);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, offScreenData.texColorBuffer, 0);

	glGenRenderbuffers(1, &offScreenData.rbo);
	glBindRenderbuffer(GL_RENDERBUFFER, offScreenData.rbo);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, 1280, 720);
	glBindRenderbuffer(GL_RENDERBUFFER, 0);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, offScreenData.rbo);

	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		std::cout << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!" << std::endl;
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void RP_Forward::Render(Camera* mainCamera_, RenderData * renderData_){
	int lightCount = renderData_->lights.size();
	int rdrCount = renderData_->renderers.size();

	//Matrice Setup
	mainCamera_->ComputeMatrix();
	for (int loop = 0; loop < rdrCount; loop++) {
		renderData_->renderers[loop]->ComputeMatrix();
	}

	//Render ShadowMap
	for (int loop = 0; loop < lightCount; loop++) {
		if (renderData_->lights[loop]->isShadowCaster == false)continue;

		renderData_->lights[loop]->EnableShadowMapBuffer();
		for (int loop2 = 0; loop2 < rdrCount; loop2++) {
			renderData_->renderers[loop2]->RenderShadowMap(renderData_->lights[loop]);
		}
	}

	//Render Off Screen	
	EnableOffSreenBuffer(mainCamera_);
	for (int loop = 0; loop < rdrCount; loop++) {
		renderData_->renderers[loop]->Render(mainCamera_, renderData_->lights);
	}
	mainCamera_->RenderSkyBox();	

	PerformanceCheck::OnEndFrame();

	//Render on screen
	glCullFace(GL_BACK);
	PostDraw();

}

void RP_Forward::Initialize() {
	InitOffScreenDraw();
}

void RP_Forward::EnableOffSreenBuffer(Camera* cam_) {
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	glViewport(
		0, 0, 
		GameWindow::GetWidth() * cam_->normalizedViewPort.x,
		GameWindow::GetHeight() * cam_->normalizedViewPort.y
	);
	glBindFramebuffer(GL_FRAMEBUFFER, offScreenData.frameBuffer);

	glEnable(GL_DEPTH_TEST);
	glDisable(GL_STENCIL_TEST);
	glClearColor(
		cam_->clearColor.x,
		cam_->clearColor.y,
		cam_->clearColor.z,
		cam_->clearColor.w
	);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
}

void RP_Forward::PostDraw() {
	glPolygonMode(GL_FRONT, GL_FILL);

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glDisable(GL_DEPTH_TEST);
	glClearColor(0.0f, 0.2f, 0.2f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);

	offScreenData.screenShader->Use();
	glBindVertexArray(offScreenData.quadVAO);
	glBindTexture(GL_TEXTURE_2D, offScreenData.texColorBuffer);
	glDrawArrays(GL_TRIANGLES, 0, 6);
}
