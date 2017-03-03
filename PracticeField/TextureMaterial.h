#pragma once
#include <string.h>

class TextureMaterial
{
private:
	char* filePath;

public:
	TextureMaterial();
	~TextureMaterial();
	
	char* GetFilePath();
};

