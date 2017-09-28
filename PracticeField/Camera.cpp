#include "Camera.h"

#include <gl\glew.h>
#include "Transform.h"
#include "InputModule.h"
#include "GameWindow.h"
#include "Time.h"
#include "Shader.h"
#include "FileManager.h"



float Camera::quadVertices[] = { // vertex attributes for a quad that fills the entire screen in Normalized Device Coordinates.
						 // positions   // texCoords
	-1.0f,  1.0f,  0.0f, 1.0f,
	-1.0f, -1.0f,  0.0f, 0.0f,
	1.0f, -1.0f,  1.0f, 0.0f,

	-1.0f,  1.0f,  0.0f, 1.0f,
	1.0f, -1.0f,  1.0f, 0.0f,
	1.0f,  1.0f,  1.0f, 1.0f
};

Camera::Camera(){
	std::cout << "Initialize Camera..." << std::endl;

	transform = new Transform();
	transform->position = glm::vec3(0, 10, 20);
	projMode = PROJECTION_PERSPECTIVE;
	fov = 0.8f;

	near = 0.1f;
	far = 10000.0f;

	verticalAngle = 0;
	horizontalAngle = 3.14f;

	moveSpeed = 20.0f;
	sensitivity = 0.001f;

	projectionMatrix = glm::perspective(fov, (float)GameWindow::GetWidth() / (float)GameWindow::GetHeight(), near, far);

	InitOffScreenDraw();
}

Camera::~Camera(){
}

void Camera::InitOffScreenDraw(){
	offScreenData.screenShader = FileManager::LoadShader("defaultScreen.vert", "defaultScreen.frag");
	offScreenData.screenShader->Use();
	glUniform1i(offScreenData.screenShader->GetUniformLocation("screenTexture"), 0);

	glGenVertexArrays(1, &offScreenData.quadVAO);
	glGenBuffers(1, &offScreenData.quadVBO);
	glBindVertexArray(offScreenData.quadVAO);
	glBindBuffer(GL_ARRAY_BUFFER, offScreenData.quadVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));

	glGenFramebuffers(1, &offScreenData.frameBuffer);
	glBindFramebuffer(GL_FRAMEBUFFER, offScreenData.frameBuffer);

	glGenTextures(1, &offScreenData.texColorBuffer);
	glBindTexture(GL_TEXTURE_2D, offScreenData.texColorBuffer);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 1280, 720, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glBindTexture(GL_TEXTURE_2D, 0);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, offScreenData.texColorBuffer, 0);

	glGenRenderbuffers(1, &offScreenData.rbo);
	glBindRenderbuffer(GL_RENDERBUFFER, offScreenData.rbo);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, 1280, 720);
	glBindRenderbuffer(GL_RENDERBUFFER, 0);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, offScreenData.rbo);

	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		std::cout << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!" << std::endl;
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
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
		transform->position,           // Camera is here
		transform->position + dirForward, // and looks here : at the same position, plus "direction"
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

void Camera::Update(){
	if (GameWindow::IsFocused()) {
		horizontalAngle += sensitivity * float(GameWindow::GetWidth() / 2 - InputModule::GetMousePos()[0]);
		verticalAngle += sensitivity * float(GameWindow::GetHeight() / 2 - InputModule::GetMousePos()[1]);

		if (InputModule::IsPressed(GLFW_KEY_W)) {
			transform->position += dirForward * Time::deltaTime * moveSpeed;
		}
		if (InputModule::IsPressed(GLFW_KEY_S)) {
			transform->position -= dirForward * Time::deltaTime * moveSpeed;
		}
		if (InputModule::IsPressed(GLFW_KEY_D)) {
			transform->position += right * Time::deltaTime * moveSpeed;
		}
		if (InputModule::IsPressed(GLFW_KEY_A)) {
			transform->position -= right * Time::deltaTime * moveSpeed;
		}
		if (InputModule::IsPressed(GLFW_KEY_Q)) {
			transform->position += Time::deltaTime * glm::vec3(0, 1, 0) * moveSpeed;
		}
		if (InputModule::IsPressed(GLFW_KEY_E)) {
			transform->position -= Time::deltaTime * glm::vec3(0, 1, 0) * moveSpeed;
		}
	}
	else {
	}

	ComputeMatrix();
}

void Camera::EnableOffSreenBuffer(){
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	glViewport(0, 0, GameWindow::GetWidth(), GameWindow::GetHeight());
	glBindFramebuffer(GL_FRAMEBUFFER, offScreenData.frameBuffer);
	
	glEnable(GL_DEPTH_TEST);
	glDisable(GL_STENCIL_TEST);
	glClearColor(0.0f, 0.2f, 0.2f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
}

void Camera::PostDraw(){
	glPolygonMode(GL_FRONT, GL_FILL);

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glDisable(GL_DEPTH_TEST);
	glClearColor(0, 1.0f, 1.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);

	offScreenData.screenShader->Use();	
	glBindVertexArray(offScreenData.quadVAO);
	glBindTexture(GL_TEXTURE_2D, offScreenData.texColorBuffer);
	glDrawArrays(GL_TRIANGLES, 0, 6);
}
