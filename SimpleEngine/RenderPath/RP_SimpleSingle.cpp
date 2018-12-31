#include "RP_SimpleSingle.h"
#include "../Scenes/SceneIncludes.h"
#include "../GameWindow.h"

#include "../Render/RenderData.h"

//Simplest RenderPath
//No Shadows, Single Pass

RP_SimpleSingle::RP_SimpleSingle() {
}


RP_SimpleSingle::~RP_SimpleSingle() {
}

void RP_SimpleSingle::Render(Camera* mainCamera_, SceneRenderData * sceneRenderData_) {
	glClearColor(
		mainCamera_->clearColor.x,
		mainCamera_->clearColor.y,
		mainCamera_->clearColor.z,
		mainCamera_->clearColor.w
	);

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

	int lightCount = sceneRenderData_->lights.size();
	int rdrCount = sceneRenderData_->renderers.size();

	//Matrice Setup
	mainCamera_->ComputeMatrix();
	for (int loop = 0; loop < rdrCount; loop++) {
		sceneRenderData_->renderers[loop]->ComputeMatrix(mainCamera_);
	}
	
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	RenderData rd;
	rd.camera = mainCamera_;
	rd.lights = &(sceneRenderData_->lights);

	for (int loop = 0; loop < rdrCount; loop++) {
		sceneRenderData_->renderers[loop]->Render(&rd);
	}
	mainCamera_->RenderSkyBox();

	PerformanceCheck::OnEndFrame();	
}