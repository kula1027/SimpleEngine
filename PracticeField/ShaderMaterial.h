#pragma once

#include <iostream>
#include <gl\glew.h>

class ShaderMaterial
{
private:
	GLuint shaderId;
	char* filePath;
public:
	ShaderMaterial(GLuint shaderId_, const char* filePath_);
	~ShaderMaterial();
	char* GetFilePath();
	int GetID();
};

