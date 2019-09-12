#pragma once

#include <Shaders/Forward/ShaderForward.h>


class InstancedShader :	public ShaderForward {
public:
	InstancedShader();
	~InstancedShader();

	virtual void Initialize();
	//virtual void SetUniforms(RenderData* renderData_, glm::mat4 modelMat_, glm::mat4 mvpMat_);
};

