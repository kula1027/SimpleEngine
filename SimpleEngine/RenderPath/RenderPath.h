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
	SceneRenderData* currentSrd;

	Camera* targetCamera;
	OffScreenData offScreenData;
	void InitOffScreenDraw();
	void EnableOffSreenBuffer(Camera* cam_);
	
	void DrawOffScreenQuad();
	void CopyFboDepth(int srcFbo, int destFbo);

public:
	RenderPath();
	~RenderPath();	
	
	virtual void Render(SceneRenderData* renderData) = 0;

	void SetTargetCamera(Camera* camera_);
};
