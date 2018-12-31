#include "Camera.h"

#include <gl\glew.h>
#include "BasesBundle.h"
#include "../GameWindow.h"
#include "../Render/Shaders/BaseShader.h"
#include "../FilePooler.h"

#include "../RenderPath/RenderPathBundle.h"


Camera::Camera(){
	name = "Camera";	

	transform->position = glm::vec3(0, 0, 0);
	clearColor = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
	projMode = PROJECTION_PERSPECTIVE;
	
	fov = 1.0f;
	near = 0.1f;
	far = 1000.0f;

	normalizedViewPort.x = 1;
	normalizedViewPort.y = 1;
	clearColor = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);

	skybox = new EmptySkyBox();
	renderMode = RenderPath_Forward;
}

Camera::~Camera(){
}


void Camera::Initialize() {
	std::cout << "Initialize Camera..." << std::endl;

	switch (projMode) {
	case PROJECTION_PERSPECTIVE:
		projectionMatrix = glm::perspective(fov, (float)GameWindow::GetWidth() / (float)GameWindow::GetHeight(), near, far);
		break;

	case PROJECTION_ORTHO:
		//projectionMatrix = glm::ortho(fov, (float)GameWindow::GetWidth() / (float)GameWindow::GetHeight(), near, far);
		//TODO
		break;

	default:
		projectionMatrix = glm::perspective(fov, (float)GameWindow::GetWidth() / (float)GameWindow::GetHeight(), near, far);
		break;
	}
	
	switch (renderMode) {
	case RenderPath_Forward:
		renderPath = new RP_Forward();
		break;

	case RenderPath_Deferred:
		//renderPipeLine = new RP_Deferred();
		break;

	case RenderPath_SimpleSingle:
		renderPath = new RP_SimpleSingle();
		break;

	default:
		renderPath = new RP_Forward();
		break;
	}

	renderPath->Initialize();
	skybox->Initialize();
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
