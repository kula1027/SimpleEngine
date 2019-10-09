#include "Camera.h"

#include <gl\glew.h>
#include <Debugger/SP_Debugger.h>
#include "BasesBundle.h"
#include "../GameWindow.h"
#include <Shaders/BaseShader.h>
#include "../FilePooler.h"

#include "../RenderPath/RenderPathBundle.h"


void Camera::InitUbo() {
	glGenBuffers(1, &uboCamera);
	glBindBuffer(GL_UNIFORM_BUFFER, uboCamera);
	glBufferData(GL_UNIFORM_BUFFER, 192, NULL, GL_STATIC_DRAW);
	glBindBufferBase(GL_UNIFORM_BUFFER, BindingPointCameraData, uboCamera);

}

Camera::Camera(){
	DebugLog("Initialize Camera...");

	InitUbo();

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

	renderPath = new RP_Deferred();	
	renderPath->SetTargetCamera(this);
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
	renderPath->Render(sceneRenderData_);
}

void Camera::SetUpMatrices(){
	upVector = glm::cross(transform->GetRight(), transform->GetForward());

	viewMatrix = glm::lookAt(
		transform->position,           // Camera is here
		transform->position + transform->GetForward(), // and looks here : at the same position, plus "direction"
		upVector                  // Head is up (set to 0,-1,0 to look upside-down)
		);	

	vpMatrix = projectionMatrix * viewMatrix * glm::mat4(1.0);	

	glBindBuffer(GL_UNIFORM_BUFFER, uboCamera);
	glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(glm::mat4), &viewMatrix);//View
	glBufferSubData(GL_UNIFORM_BUFFER, sizeof(glm::mat4), sizeof(glm::mat4), &projectionMatrix);//Projection
	glBufferSubData(GL_UNIFORM_BUFFER, sizeof(glm::mat4) * 2, sizeof(glm::mat4), &vpMatrix);//P x V	
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
