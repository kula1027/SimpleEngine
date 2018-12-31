#include "MoveCamera.h"

#include <gl\glew.h>
#include "../GameWindow.h"

#include "../Bases/BasesBundle.h"

MoveCamera::MoveCamera(){

}


MoveCamera::~MoveCamera()
{
}

void MoveCamera::OnUpdate(){			
	if (GameWindow::IsFocused()) {
		horizontalAngle += sensitivity * float(GameWindow::GetWidth() / 2 - InputModule::GetMousePos()[0]);
		verticalAngle += sensitivity * float(GameWindow::GetHeight() / 2 - InputModule::GetMousePos()[1]);

		if (InputModule::IsPressed(GLFW_KEY_W)) {			
			transform->position += cam->transform->GetForward() * Time::deltaTime * moveSpeed;
		}
		if (InputModule::IsPressed(GLFW_KEY_S)) {
			transform->position -= cam->transform->GetForward() * Time::deltaTime * moveSpeed;
		}
		if (InputModule::IsPressed(GLFW_KEY_D)) {
			transform->position += cam->transform->GetRight() * Time::deltaTime * moveSpeed;
		}
		if (InputModule::IsPressed(GLFW_KEY_A)) {
			transform->position -= cam->transform->GetRight() * Time::deltaTime * moveSpeed;
		}
		if (InputModule::IsPressed(GLFW_KEY_Q)) {
			transform->position += Time::deltaTime * glm::vec3(0, 1, 0) * moveSpeed;
		}
		if (InputModule::IsPressed(GLFW_KEY_E)) {
			transform->position -= Time::deltaTime * glm::vec3(0, 1, 0) * moveSpeed;
		}

		cam->transform->SetForward(
			glm::vec3(
				cos(verticalAngle) * sin(horizontalAngle),
				sin(verticalAngle),
				cos(verticalAngle) * cos(horizontalAngle)
				)
			);

		cam->transform->SetRight(
			glm::vec3(
				sin(horizontalAngle - 3.14f / 2.0f),
				0,
				cos(horizontalAngle - 3.14f / 2.0f)
			)
		);
	}

	if (InputModule::IsPressed(GLFW_KEY_KP_ENTER)) {
		std::cout << "Cam Tr: " <<
			transform->position.x << ", " <<
			transform->position.y << ", " <<
			transform->position.z << " / " <<
			horizontalAngle << ", " << verticalAngle <<
			endl;

	}
	
}

void MoveCamera::OnStart(){
	cam = static_cast<Camera*>(gameObject);

	horizontalAngle = 3.14f;
	verticalAngle = 0.f;
	moveSpeed = 20.0f;
	sensitivity = 0.001f;
}
