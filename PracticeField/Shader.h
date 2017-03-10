#pragma once

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

#include <gl\glew.h>


class Shader
{
private:
	GLuint shaderID;

public:
	Shader(GLchar* vertexPath, GLchar* fragmentPath);
	~Shader();
	char* GetFilePath();
	int GetID();
};

