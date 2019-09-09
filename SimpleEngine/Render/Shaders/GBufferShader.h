#pragma once
#include "BaseShader.h"

class GBufferShader : public BaseShader {
public:
	GBufferShader();
	~GBufferShader();

	ID_matrice id_matrice;
};

