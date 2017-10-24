#include "MoveCamera.h"
#include <gl\glew.h>
#include "InputModule.h"
#include "GameWindow.h"
#include "Time.h"
#include "GameObject.h"
#include "Camera.h"

MoveCamera::MoveCamera()
{
}


MoveCamera::~MoveCamera()
{
}

void MoveCamera::OnUpdate(){			
	if (GameWindow::IsFocused()) {
		horizontalAngle += sensitivity * float(GameWindow::GetWidth() / 2 - InputModule::GetMousePos()[0]);
		verticalAngle += sensitivity * float(GameWindow::GetHeight() / 2 - InputModule::GetMousePos()[1]);

		if (InputModule::IsPressed(GLFW_KEY_W)) {			
			transform->position += cam->transform->forward * Time::deltaTime * moveSpeed;
		}
		if (InputModule::IsPressed(GLFW_KEY_S)) {
			transform->position -= cam->transform->forward * Time::deltaTime * moveSpeed;
		}
		if (InputModule::IsPressed(GLFW_KEY_D)) {
			transform->position += cam->transform->right * Time::deltaTime * moveSpeed;
		}
		if (InputModule::IsPressed(GLFW_KEY_A)) {
			transform->position -= cam->transform->right * Time::deltaTime * moveSpeed;
		}
		if (InputModule::IsPressed(GLFW_KEY_Q)) {
			transform->position += Time::deltaTime * glm::vec3(0, 1, 0) * moveSpeed;
		}
		if (InputModule::IsPressed(GLFW_KEY_E)) {
			transform->position -= Time::deltaTime * glm::vec3(0, 1, 0) * moveSpeed;
		}

		cam->transform->forward = glm::vec3(
			cos(verticalAngle) * sin(horizontalAngle),
			sin(verticalAngle),
			cos(verticalAngle) * cos(horizontalAngle)
		);

		cam->transform->right = glm::vec3(
			sin(horizontalAngle - 3.14f / 2.0f),
			0,
			cos(horizontalAngle - 3.14f / 2.0f)
		);
	}

	
}

void MoveCamera::OnStart(){
	cam = static_cast<Camera*>(gameObject);

	verticalAngle = 0;
	horizontalAngle = 3.14f;
	moveSpeed = 10.0f;
	sensitivity = 0.001f;
}
