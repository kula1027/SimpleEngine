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
			transform->AddPosition(cam->GetTransform()->GetForward() * SP_Time::deltaTime * moveSpeed);
		}
		if (InputModule::IsPressed(GLFW_KEY_S)) {
			transform->AddPosition(-cam->GetTransform()->GetForward() * SP_Time::deltaTime * moveSpeed);
		}
		if (InputModule::IsPressed(GLFW_KEY_D)) {
			transform->AddPosition(cam->GetTransform()->GetRight() * SP_Time::deltaTime * moveSpeed);			
		}
		if (InputModule::IsPressed(GLFW_KEY_A)) {
			transform->AddPosition(-cam->GetTransform()->GetRight() * SP_Time::deltaTime * moveSpeed);			
		}
		if (InputModule::IsPressed(GLFW_KEY_Q)) {
			transform->AddPosition(glm::vec3(0, 1, 0) * SP_Time::deltaTime * moveSpeed);			
		}
		if (InputModule::IsPressed(GLFW_KEY_E)) {
			transform->AddPosition(glm::vec3(0, -1, 0) * SP_Time::deltaTime * moveSpeed);
		}

		cam->GetTransform()->SetForward(
			glm::vec3(
				cos(verticalAngle) * sin(horizontalAngle),
				sin(verticalAngle),
				cos(verticalAngle) * cos(horizontalAngle)
				)
			);

		cam->GetTransform()->SetRight(
			glm::vec3(
				sin(horizontalAngle - 3.14f / 2.0f),
				0,
				cos(horizontalAngle - 3.14f / 2.0f)
			)
		);
	}

	if (InputModule::IsPressed(GLFW_KEY_KP_ENTER)) {
		std::cout << "Cam Tr: " <<
			transform->GetPosition().x << ", " <<
			transform->GetPosition().y << ", " <<
			transform->GetPosition().z << " / " <<
			horizontalAngle << ", " << verticalAngle <<
			endl;

	}
	
}

void MoveCamera::OnStart(){
	cam = static_cast<Camera*>(engineObject);

	horizontalAngle = 3.14f;
	verticalAngle = 0.f;
	moveSpeed = 20.0f;
	sensitivity = 0.001f;
}
