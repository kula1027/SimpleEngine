#pragma once


#include <string>
#include <gl\glew.h>

#include "EngineResource.h"

#define LogLength 512

#define TEXTURE_IDX_SHADOWMAP 10

class Shader : EngineResource
{
private:
	GLuint shaderID;
	std::string ReadCodeFromFile(std::string path_);
	void CompileCode(int shaderId_, const GLchar* code_);
	GLuint CreateShader(GLchar* path_, int shaderType_);
	void CreateProgram(GLuint shader0, GLuint shader1, GLuint shader2);
	void CreateProgram(GLuint shader0, GLuint shader1);

public:
	Shader(GLchar* vertexPath_, GLchar* fragmentPath_);
	Shader(GLchar* vertexPath_, GLchar* geometryPath_, GLchar* fragmentPath_);
	~Shader();
	char* GetFilePath();
	
	void Use();
	GLuint GetUniformLocation(GLchar* var_name);

	virtual void OnEndUse();
};

