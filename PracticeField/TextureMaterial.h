#pragma once
#include <string.h>

class TextureMaterial
{
public:
	TextureMaterial();
	~TextureMaterial();

	void LoadTextures();
	void GetTexture(const char* textureName);
};

