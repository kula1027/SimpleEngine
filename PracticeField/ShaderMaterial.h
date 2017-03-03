#pragma once

#include "ShaderLoader.h"
#include <gl\glew.h>

class ShaderMaterial
{
private:
	GLuint shaderId;
	char* name;
public:
	ShaderMaterial();
	~ShaderMaterial();
};

