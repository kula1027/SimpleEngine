#pragma once

class RenderData;
class Camera;
class Shader;


#include <glm\glm.hpp>

struct OffScreenData {
	Shader* screenShader;
	unsigned int texColorBuffer;
	unsigned int frameBuffer;
	unsigned int quadVAO, quadVBO;
	unsigned int rbo;	
};

class RenderPipeLine{
protected:	
	static float quadVertices[];
	static int quadSize;

public:
	RenderPipeLine();
	~RenderPipeLine();	

	virtual void Render(Camera* mainCamera, RenderData* renderData) = 0;
};
