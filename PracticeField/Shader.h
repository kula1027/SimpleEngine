#pragma once

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

#include <gl\glew.h>

#define TEXTURE_IDX_SHADOWMAP 10

class Shader
{
private:
	GLuint shaderID;

public:
	Shader(GLchar* vertexPath, GLchar* fragmentPath);
	~Shader();
	char* GetFilePath();
	
	void Use();
	GLuint GetUniformLocation(GLchar* var_name);
};

