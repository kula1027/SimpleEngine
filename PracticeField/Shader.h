#pragma once



#include <gl\glew.h>

#include "EngineResource.h"

#define TEXTURE_IDX_SHADOWMAP 10

class Shader : EngineResource
{
private:
	GLuint shaderID;

public:
	Shader(GLchar* vertexPath, GLchar* fragmentPath);
	~Shader();
	char* GetFilePath();
	
	void Use();
	GLuint GetUniformLocation(GLchar* var_name);

	virtual void OnEndUse();
};

