#pragma once

class SceneRenderData;
class Camera;
class BaseShader;

#define RenderPath_Forward 0
#define RenderPath_Deferred 1
#define RenderPath_SimpleSingle 2

#include <glm\glm.hpp>

struct OffScreenData {
	BaseShader* screenShader;
	unsigned int texColorBuffer;
	unsigned int frameBuffer;
	unsigned int quadVAO, quadVBO;
	unsigned int rbo;	
};

class RenderPath{
protected:	
	static float quadVertices[];
	static int quadSize;

public:
	RenderPath();
	~RenderPath();	

	virtual void Initialize() {}

	virtual void Render(Camera* mainCamera, SceneRenderData* renderData) = 0;
};
