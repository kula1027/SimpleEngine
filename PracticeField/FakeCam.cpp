#include "FakeCam.h"
#include "GameObject.h"
#include "InputModule.h"
#include "Time.h"

FakeCam::FakeCam()
{
}


FakeCam::~FakeCam()
{
}

void FakeCam::OnUpdate()
{
	if (InputModule::IsPressed(GLFW_KEY_KP_2)) {
		transform->position += transform->GetForward() * Time::deltaTime * moveSpeed;
	}
	if (InputModule::IsPressed(GLFW_KEY_KP_5)) {
		transform->position -= transform->GetForward() * Time::deltaTime * moveSpeed;
	}
	if (InputModule::IsPressed(GLFW_KEY_KP_3)) {
		transform->position += transform->GetRight() * Time::deltaTime * moveSpeed;
	}
	if (InputModule::IsPressed(GLFW_KEY_KP_1)) {
		transform->position -= transform->GetRight() * Time::deltaTime * moveSpeed;
	}

	if (InputModule::IsPressed(GLFW_KEY_KP_6)) {
		transform->position -= transform->GetUp() * Time::deltaTime * moveSpeed;
	}
	if (InputModule::IsPressed(GLFW_KEY_KP_4)) {
		transform->position += transform->GetUp() * Time::deltaTime * moveSpeed;
	}
}
