#pragma once




class InstancedShader {
public:
	InstancedShader();
	~InstancedShader();

	virtual void Initialize();
	//virtual void SetUniforms(RenderData* renderData_, glm::mat4 modelMat_, glm::mat4 mvpMat_);
};

