#include "FakeCam.h"


FakeCam::FakeCam()
{
}


FakeCam::~FakeCam()
{
}

void FakeCam::OnUpdate()
{
	if (InputModule::IsPressed(GLFW_KEY_KP_2)) {
		transform->position += transform->GetForward() * SP_Time::deltaTime * moveSpeed;
	}
	if (InputModule::IsPressed(GLFW_KEY_KP_5)) {
		transform->position -= transform->GetForward() * SP_Time::deltaTime * moveSpeed;
	}
	if (InputModule::IsPressed(GLFW_KEY_KP_3)) {
		transform->position += transform->GetRight() * SP_Time::deltaTime * moveSpeed;
	}
	if (InputModule::IsPressed(GLFW_KEY_KP_1)) {
		transform->position -= transform->GetRight() * SP_Time::deltaTime * moveSpeed;
	}

	if (InputModule::IsPressed(GLFW_KEY_KP_6)) {
		transform->position -= transform->GetUp() * SP_Time::deltaTime * moveSpeed;
	}
	if (InputModule::IsPressed(GLFW_KEY_KP_4)) {
		transform->position += transform->GetUp() * SP_Time::deltaTime * moveSpeed;
	}	

	if (InputModule::IsPressed(GLFW_KEY_KP_ENTER)) {
		std::cout << "fCam Pos: " <<
			transform->position.x << ", " <<
			transform->position.y << ", " <<
			transform->position.z <<
			endl;

	}
}
