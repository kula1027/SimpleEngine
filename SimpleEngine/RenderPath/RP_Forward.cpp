#include "RP_Forward.h"

#include "../Scenes/SceneIncludes.h"
#include <GameWindow.h>


RP_Forward::RP_Forward() {
	
}

RP_Forward::~RP_Forward() {
}



void RP_Forward::Render(Camera* mainCamera_, SceneRenderData * sceneRenderData_){
	//int lightCount = sceneRenderData_->lights.size();
	//int rdrCount = sceneRenderData_->renderQueue_Deferred.size();

	////Matrice Setup
	//mainCamera_->ComputeMatrix();
	//for (int loop = 0; loop < rdrCount; loop++) {
	//	sceneRenderData_->renderQueue_Deferred[loop]->ComputeMatrix(mainCamera_);
	//}

	////Render ShadowMap
	//for (int loop = 0; loop < lightCount; loop++) {
	//	if (sceneRenderData_->lights[loop]->isShadowCaster == false)continue;

	//	sceneRenderData_->lights[loop]->EnableShadowMapBuffer();
	//	for (int loop2 = 0; loop2 < rdrCount; loop2++) {
	//		sceneRenderData_->renderQueue_Deferred[loop2]->RenderShadowMap(sceneRenderData_->lights[loop]);
	//	}
	//}

	////Enable Off Screen fb
	//glViewport(
	//	0, 0,
	//	GameWindow::GetWidth() * mainCamera_->normalizedViewPort.x,
	//	GameWindow::GetHeight() * mainCamera_->normalizedViewPort.y
	//);
	//glBindFramebuffer(GL_FRAMEBUFFER, offScreenData.frameBuffer);

	//glEnable(GL_DEPTH_TEST);
	//glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

	////Render Off Screen	
	//for (int loop = 0; loop < rdrCount; loop++) {		
	//	sceneRenderData_->renderQueue_Deferred[loop]->RenderMesh_Forward(mainCamera_, &sceneRenderData_->lights);
	//}
	//mainCamera_->RenderSkyBox();	

	////Render on off rendered texture to default fb
	//glBindFramebuffer(GL_FRAMEBUFFER, 0);
	//glDisable(GL_DEPTH_TEST);
	//glClearColor(0.0f, 0.7f, 0.2f, 1.0f);
	//glClear(GL_COLOR_BUFFER_BIT);

	//offScreenData.screenShader->Use();
	//glBindVertexArray(offScreenData.quadVAO);

	//glActiveTexture(GL_TEXTURE0);
	//glBindTexture(GL_TEXTURE_2D, offScreenData.texColorBuffer);
	//glDrawArrays(GL_TRIANGLES, 0, 6);
}

void RP_Forward::Initialize() {
	InitOffScreenDraw();
	
	glEnable(GL_CULL_FACE);
}
