#pragma once

class SceneRenderData;
class Camera;
class BaseShader;


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

	OffScreenData offScreenData;
	void InitOffScreenDraw();
	void EnableOffSreenBuffer(Camera* cam_);

	void PostDraw();

public:
	RenderPath();
	~RenderPath();	

	virtual void Initialize() {}

	virtual void Render(Camera* mainCamera, SceneRenderData* renderData) = 0;
};
