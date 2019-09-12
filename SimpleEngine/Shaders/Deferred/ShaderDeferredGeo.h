#pragma once
#include "../BaseShader.h"

class ShaderDeferredGeo : public BaseShader {
public:
	ShaderDeferredGeo();
	~ShaderDeferredGeo();

	virtual void Initialize() override;

	ID_matrice id_matrice;
};

