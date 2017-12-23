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
		transform->position += transform->forward * Time::deltaTime * moveSpeed;
	}
	if (InputModule::IsPressed(GLFW_KEY_K)) {
		transform->position -= transform->forward * Time::deltaTime * moveSpeed;
	}
	if (InputModule::IsPressed(GLFW_KEY_L)) {
		transform->position += transform->right * Time::deltaTime * moveSpeed;
	}
	if (InputModule::IsPressed(GLFW_KEY_J)) {
		transform->position -= transform->right * Time::deltaTime * moveSpeed;
	}

}
