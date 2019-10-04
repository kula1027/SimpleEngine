#include "Camera.h"

#include <gl\glew.h>
#include <Debugger/SP_Debugger.h>
#include "BasesBundle.h"
#include "../GameWindow.h"
#include <Shaders/BaseShader.h>
#include "../FilePooler.h"

#include "../RenderPath/RenderPathBundle.h"


Camera::Camera(){
	DebugLog("Initialize Camera...");

	name = "Camera";	

	transform->position = glm::vec3(0, 0, 0);
	clearColor = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);

	fov = 1.0f;
	near = 0.3f;
	far = 1000.0f;

	normalizedViewPort.x = 1;
	normalizedViewPort.y = 1;
	clearColor = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);

	skybox = new EmptySkyBox();

	projMode = PROJECTION_PERSPECTIVE;
	switch (projMode) {
	case PROJECTION_PERSPECTIVE:
		projectionMatrix = glm::perspective(fov, (float)GameWindow::GetWidth() / (float)GameWindow::GetHeight(), near, far);
		break;

	case PROJECTION_ORTHO:
		//projectionMatrix = glm::ortho(fov, (float)GameWindow::GetWidth() / (float)GameWindow::GetHeight(), near, far);
		//TODO
		break;

	default:
		DebugError("Projection mode not defined.");		
		projectionMatrix = glm::perspective(fov, (float)GameWindow::GetWidth() / (float)GameWindow::GetHeight(), near, far);
		break;
	}

	renderMode = DefaultRenderMode;
	switch (renderMode) {
	case RenderMode_Forward:
		renderPath = new RP_Forward();
		break;

	case RenderMode_Deferred:
		renderPath = new RP_Deferred();
		break;

	case RenderMode_SimpleSingle:
		renderPath = new RP_SimpleSingle();
		break;

	default:
		DebugError("RenderPath not defined.");
		renderPath = new RP_Forward();
		break;
	}

	renderPath->Initialize();
	skybox->Initialize();
}

Camera::~Camera(){
}


void Camera::SetSkybox(SkyBox * skybox_) {
	if (skybox != NULL) {
		free(skybox);
	}

	skybox = skybox_;
	skybox->Initialize();
}

SkyBox * Camera::GetSkybox() {
	return skybox;
}

void Camera::Render(SceneRenderData* sceneRenderData_) {
	renderPath->Render(this, sceneRenderData_);
}

void Camera::ComputeMatrix(){
	upVector = glm::cross(transform->GetRight(), transform->GetForward());

	viewMatrix = glm::lookAt(
		transform->position,           // Camera is here
		transform->position + transform->GetForward(), // and looks here : at the same position, plus "direction"
		upVector                  // Head is up (set to 0,-1,0 to look upside-down)
		);

	vpMatrix = projectionMatrix * viewMatrix * glm::mat4(1.0);	
}

glm::mat4 Camera::VPmatrix() {
	return vpMatrix;
}

glm::mat4 Camera::Vmatrix() {
	return viewMatrix;
}

glm::mat4 Camera::Pmatrix() {
	return projectionMatrix;
}

void Camera::RenderSkyBox(){
	skybox->Render(this);
}
