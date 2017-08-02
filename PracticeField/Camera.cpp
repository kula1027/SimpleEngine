#include "Camera.h"


Camera::Camera(){
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);
	glEnable(GL_CULL_FACE);

	transform.position = glm::vec3(0, 0, 40);
	projMode = PROJECTION_PERSPECTIVE;
	fov = 45.0f;

	near = 0.1f;
	far = 1000.0f;

	verticalAngle = 0;
	horizontalAngle = 3.14f;

	moveSpeed = 10.0f;
	sensitivity = 0.001f;

	projectionMatrix = glm::perspective(fov, (float)GameWindow::GetWidth() / (float)GameWindow::GetHeight(), near, far);

}

Camera::~Camera(){
}

void Camera::ComputeMatrix(){	
	dirForward = glm::vec3(
		cos(verticalAngle) * sin(horizontalAngle),
		sin(verticalAngle),
		cos(verticalAngle) * cos(horizontalAngle)
		);

	right = glm::vec3(
		sin(horizontalAngle - 3.14f / 2.0f),
		0,
		cos(horizontalAngle - 3.14f / 2.0f)
		);

	upVector = glm::cross(right, dirForward);

	viewMatrix = glm::lookAt(
		transform.position,           // Camera is here
		transform.position + dirForward, // and looks here : at the same position, plus "direction"
		upVector                  // Head is up (set to 0,-1,0 to look upside-down)
		);

	mvpMatrix = projectionMatrix * viewMatrix * glm::mat4(1.0);	
}

glm::mat4 Camera::VPmatrix() {
	return mvpMatrix;
}

glm::mat4 Camera::Vmatrix() {
	return viewMatrix;
}

void Camera::Update(){
	glClearColor(0.2f, 0.2f, 0.2f, 1);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	horizontalAngle += sensitivity * float(GameWindow::GetWidth() / 2 - InputModule::GetMousePos()[0]);
	verticalAngle += sensitivity * float(GameWindow::GetHeight() / 2 - InputModule::GetMousePos()[1]);

	if (InputModule::IsPressed(GLFW_KEY_W)) {
		transform.position += dirForward * Time::deltaTime * moveSpeed;
	}
	if (InputModule::IsPressed(GLFW_KEY_S)) {
		transform.position -= dirForward * Time::deltaTime * moveSpeed;
	}
	if (InputModule::IsPressed(GLFW_KEY_D)) {
		transform.position += right * Time::deltaTime * moveSpeed;
	}
	if (InputModule::IsPressed(GLFW_KEY_A)) {
		transform.position -= right * Time::deltaTime * moveSpeed;
	}

	ComputeMatrix();
}
