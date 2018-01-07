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
	if (InputModule::IsPressed(GLFW_KEY_I)) {
		transform->position += transform->GetForward() * Time::deltaTime * moveSpeed;
	}
	if (InputModule::IsPressed(GLFW_KEY_K)) {
		transform->position -= transform->GetForward() * Time::deltaTime * moveSpeed;
	}
	if (InputModule::IsPressed(GLFW_KEY_L)) {
		transform->position += transform->GetRight() * Time::deltaTime * moveSpeed;
	}
	if (InputModule::IsPressed(GLFW_KEY_J)) {
		transform->position -= transform->GetRight() * Time::deltaTime * moveSpeed;
	}

	if (InputModule::IsPressed(GLFW_KEY_O)) {
		transform->position -= transform->GetUp() * Time::deltaTime * moveSpeed;
	}
	if (InputModule::IsPressed(GLFW_KEY_U)) {
		transform->position += transform->GetUp() * Time::deltaTime * moveSpeed;
	}
}
