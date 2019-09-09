#include "RP_Forward.h"

#include "../Scenes/SceneIncludes.h"
#include <GameWindow.h>

#include "../Render/RenderData.h"

RP_Forward::RP_Forward() {
	
}

RP_Forward::~RP_Forward() {
}



void RP_Forward::Render(Camera* mainCamera_, SceneRenderData * sceneRenderData_){
	int lightCount = sceneRenderData_->lights.size();
	int rdrCount = sceneRenderData_->renderers.size();

	//Matrice Setup
	mainCamera_->ComputeMatrix();
	for (int loop = 0; loop < rdrCount; loop++) {
		sceneRenderData_->renderers[loop]->ComputeMatrix(mainCamera_);
	}

	////Render ShadowMap
	for (int loop = 0; loop < lightCount; loop++) {
		if (sceneRenderData_->lights[loop]->isShadowCaster == false)continue;

		sceneRenderData_->lights[loop]->EnableShadowMapBuffer();
		for (int loop2 = 0; loop2 < rdrCount; loop2++) {
			sceneRenderData_->renderers[loop2]->RenderShadowMap(sceneRenderData_->lights[loop]);
		}
	}

	//Render Off Screen	
	EnableOffSreenBuffer(mainCamera_);
	RenderData rd;//HACK: 네이밍이 이상?
	rd.camera = mainCamera_;
	rd.lights = &(sceneRenderData_->lights);
	for (int loop = 0; loop < rdrCount; loop++) {
		sceneRenderData_->renderers[loop]->Render(&rd);
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
	glClearColor(0.0f, 0.7f, 0.2f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);

	offScreenData.screenShader->Use();
	glBindVertexArray(offScreenData.quadVAO);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, offScreenData.texColorBuffer);
	glDrawArrays(GL_TRIANGLES, 0, 6);
}
