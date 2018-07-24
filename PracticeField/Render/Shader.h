#pragma once


#include <string>
#include <gl\glew.h>

#include "../Bases/EngineResource.h"

#define LogLength 512

#define dirPathShader "Materials/Shaders/"
#define TEXTURE_IDX_SHADOWMAP 10

using namespace std;

class Shader : EngineResource
{
private:
	GLuint shaderID;
	std::string ReadCodeFromFile(std::string path_);
	void CompileCode(int shaderId_, const GLchar* code_);
	GLuint CreateShader(GLchar* path_, int shaderType_);
	void CreateProgram(GLuint shader0, GLuint shader1, GLuint shader2);
	void CreateProgram(GLuint shader0, GLuint shader1);

	void LoadProgram(string vertexPath_, string geometryPath_, string fragmentPath_);

	string filePathVertex;
	string filePathGeometry;
	string filePathFragment;	

public:
	Shader(string vertexPath_, string fragmentPath_);
	Shader(string vertexPath_, string geometryPath_, string fragmentPath_);
	~Shader();
	string GetDirectoryVertex();
	string GetDirectoryGeometry();
	string GetDirectoryFragment();
	
	void Use();
	GLuint GetUniformLocation(GLchar* var_name);

	virtual void OnEndUse();
};

