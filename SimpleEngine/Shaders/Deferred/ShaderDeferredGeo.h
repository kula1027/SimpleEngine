#pragma once
#include "../BaseShader.h"

class ShaderDeferredGeo : public BaseShader {
private:
	ID_matrice id_matrice;

public:
	ShaderDeferredGeo();
	~ShaderDeferredGeo();

	
	void SetModelMat(glm::mat4 modelMa_);
};

