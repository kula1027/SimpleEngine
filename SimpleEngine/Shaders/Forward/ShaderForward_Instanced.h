#pragma once

#include <Shaders/BaseShader.h>

class ShaderForward_Instanced : public BaseShader {
public:
	ShaderForward_Instanced();
	~ShaderForward_Instanced();

	ID_matrice id_matrice;
	virtual void SetUniforms(MeshRenderer* renderer_) override;
};

