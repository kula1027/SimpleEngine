#include "Camera.h"

#include <gl\glew.h>
#include "BasesBundle.h"
#include "../GameWindow.h"
#include "../Render/Shader.h"
#include "../FilePooler.h"
#include "../RenderPipeLine/RPL_Forward.h"


Camera::Camera(){
	name = "Camera";
	std::cout << "Initialize Camera..." << std::endl;

	transform->position = glm::vec3(0, 14, 30);
	clearColor = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
	projMode = PROJECTION_PERSPECTIVE;
	
	fov = 1.0f;
	near = 0.1f;
	far = 1000.0f;

	projectionMatrix = glm::perspective(fov, (float)GameWindow::GetWidth() / (float)GameWindow::GetHeight(), near, far);

	normalizedViewPort.x = 1;
	normalizedViewPort.y = 1;
	clearColor = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);

	renderPipeLine = new RPL_Forward();
	skybox = new EmptySkyBox();
}

Camera::~Camera(){
}


void Camera::Render(RenderData* renderData_) {
	renderPipeLine->Render(this, renderData_);
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
