#include "RenderPath.h"
#include "../Scenes/SceneIncludes.h"
#include "../GameWindow.h"

#include "../Render/RenderData.h"

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
//��Ʈ: Rendering Path�� ���� �ٸ� ���·� ������Ʈ���� �����ؾ���.
//eg. ���۵� + ������ ����. ���۵�� �׷��� �Ͱ� �׷��� ���� ���� �и� �����ؾ���
//Render Path�� ī�޶� �ͼӵǴ� ����
//������ ������Ʈ ������ Scene�� ��.
//���� Scene�� Camera�� �䱸�ϴ� ������ �����͸� ��������� ��.
//1. Scene���� ��� ������ ���� �о��� �����ٰ� �ع�����.
//	�� �о��� ���� �ڷᱸ���� ���� �����Ͽ� ���

RenderPath::RenderPath(){
}


RenderPath::~RenderPath(){
}

void RenderPath::InitOffScreenDraw() {
	offScreenData.screenShader = new BaseShader("PostProcess/defaultScreen.vert", "PostProcess/defaultScreen.frag");
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

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}