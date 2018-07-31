#include "RP_SimpleSingle.h"
#include "../Scenes/SceneIncludes.h"
#include "../GameWindow.h"

//Simplest RenderPath
//No Shadows, Single Pass

RP_SimpleSingle::RP_SimpleSingle() {
}


RP_SimpleSingle::~RP_SimpleSingle() {
}

void RP_SimpleSingle::Render(Camera* mainCamera_, RenderData * renderData_) {
	glClearColor(
		mainCamera_->clearColor.x,
		mainCamera_->clearColor.y,
		mainCamera_->clearColor.z,
		mainCamera_->clearColor.w
	);

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

	int lightCount = renderData_->lights.size();
	int rdrCount = renderData_->renderers.size();

	//Matrice Setup
	mainCamera_->ComputeMatrix();
	for (int loop = 0; loop < rdrCount; loop++) {
		renderData_->renderers[loop]->ComputeMatrix();
	}
	
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	for (int loop = 0; loop < rdrCount; loop++) {
		renderData_->renderers[loop]->Render(mainCamera_, renderData_->lights);
	}
	mainCamera_->RenderSkyBox();

	PerformanceCheck::OnEndFrame();	
}