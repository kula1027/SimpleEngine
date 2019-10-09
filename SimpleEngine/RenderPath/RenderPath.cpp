#include "RenderPath.h"
#include <Scenes/SceneIncludes.h>
#include <GameWindow.h>

float RenderPath::quadVertices[] = { // vertex attributes for a quad that fills the entire screen in Normalized Device Coordinates.
										 // positions   // texCoords
	-1.0f,  1.0f,  0.0f, 1.0f,
	-1.0f, -1.0f,  0.0f, 0.0f,
	1.0f, -1.0f,  1.0f, 0.0f,

	-1.0f,  1.0f,  0.0f, 1.0f,
	1.0f, -1.0f,  1.0f, 0.0f,
	1.0f,  1.0f,  1.0f, 1.0f
};

int RenderPath::quadSize = 24;
RenderPath::RenderPath(){
}


RenderPath::~RenderPath(){
}

void RenderPath::SetTargetCamera(Camera * camera_) {
	targetCamera = camera_;
}

void RenderPath::InitOffScreenDraw() {
	offScreenData.screenShader = new BaseShader("PostProcess/defaultScreen");
	offScreenData.screenShader->Use();
	glUniform1i(offScreenData.screenShader->GetUniformLocation("screenTexture"), 0);

	glGenVertexArrays(1, &offScreenData.quadVAO);
	glGenBuffers(1, &offScreenData.quadVBO);
	glBindVertexArray(offScreenData.quadVAO);
	glBindBuffer(GL_ARRAY_BUFFER, offScreenData.quadVBO);
	glBufferData(GL_ARRAY_BUFFER, quadSize * sizeof(float), &quadVertices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));

	glGenFramebuffers(1, &offScreenData.frameBuffer);
	glBindFramebuffer(GL_FRAMEBUFFER, offScreenData.frameBuffer);

	glGenTextures(1, &offScreenData.texColorBuffer);
	glBindTexture(GL_TEXTURE_2D, offScreenData.texColorBuffer);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, GameWindow::GetWidth(), GameWindow::GetHeight(), 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glBindTexture(GL_TEXTURE_2D, 0);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, offScreenData.texColorBuffer, 0);

	glGenRenderbuffers(1, &offScreenData.rbo);
	glBindRenderbuffer(GL_RENDERBUFFER, offScreenData.rbo);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, GameWindow::GetWidth(), GameWindow::GetHeight());
	glBindRenderbuffer(GL_RENDERBUFFER, 0);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, offScreenData.rbo);

	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		std::cout << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!" << std::endl;	
}

void RenderPath::EnableOffSreenBuffer(Camera* cam_) {
	glViewport(
		0, 0,
		GameWindow::GetWidth() * cam_->normalizedViewPort.x,
		GameWindow::GetHeight() * cam_->normalizedViewPort.y
	);
	glBindFramebuffer(GL_FRAMEBUFFER, offScreenData.frameBuffer);

	glEnable(GL_DEPTH_TEST);
	glDisable(GL_STENCIL_TEST);
	glClearColor(
		cam_->clearColor.x,
		cam_->clearColor.y,
		cam_->clearColor.z,
		cam_->clearColor.w
	);

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
}

void RenderPath::DrawOffScreenQuad() {
	glBindVertexArray(offScreenData.quadVAO);
	glDrawArrays(GL_TRIANGLES, 0, 6);
}
