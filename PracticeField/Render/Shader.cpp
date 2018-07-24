#include "Shader.h"

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

std::string Shader::ReadCodeFromFile(std::string path_){
	std::string strCode;
	std::ifstream fileStream;
	fileStream.exceptions(std::ifstream::badbit);

	try {
		fileStream.open(path_);
		std::stringstream strStream;
		strStream << fileStream.rdbuf();
		fileStream.close();
		strCode = strStream.str();
	}catch (std::ifstream::failure e) {
		std::cout << "Error _ ReadCodeFromFile _ " << path_ << std::endl;
	}

	return strCode;
}

void Shader::CompileCode(int shaderId_, const GLchar* code_){
	GLint success;
	GLchar infoLog[LogLength];

	glShaderSource(shaderId_, 1, &code_, NULL);
	glCompileShader(shaderId_);

	glGetShaderiv(shaderId_, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(shaderId_, LogLength, NULL, infoLog);
		std::cout << "Error _ CompileCode\n" << infoLog << std::endl;
	}
}

GLuint Shader::CreateShader(GLchar* path_, int shaderType_){
	std::string code = ReadCodeFromFile(path_);
	const GLchar* chCode = code.c_str();

	GLuint shader;
	shader = glCreateShader(shaderType_);
	CompileCode(shader, chCode);

	return shader;
}

void Shader::CreateProgram(GLuint shader0, GLuint shader1, GLuint shader2){
	GLint success;
	GLchar infoLog[LogLength];

	// Shader Program
	this->shaderID = glCreateProgram();
	glAttachShader(this->shaderID, shader0);
	glAttachShader(this->shaderID, shader1);
	glAttachShader(this->shaderID, shader2);
	glLinkProgram(this->shaderID);
	// Print linking errors if any
	glGetProgramiv(this->shaderID, GL_LINK_STATUS, &success);
	if (!success)
	{
		glGetProgramInfoLog(this->shaderID, LogLength, NULL, infoLog);
		std::cout << "Error _ CreateProgram\n" << infoLog << std::endl;
	}
}

void Shader::CreateProgram(GLuint shader0, GLuint shader1){
	GLint success;
	GLchar infoLog[LogLength];

	// Shader Program
	this->shaderID = glCreateProgram();
	glAttachShader(this->shaderID, shader0);
	glAttachShader(this->shaderID, shader1);
	glLinkProgram(this->shaderID);
	// Print linking errors if any
	glGetProgramiv(this->shaderID, GL_LINK_STATUS, &success);
	if (!success)
	{
		glGetProgramInfoLog(this->shaderID, LogLength, NULL, infoLog);
		std::cout << "Error _ CreateProgram\n" << infoLog << std::endl;
	}
}

void Shader::LoadProgram(string vertexPath_, string geometryPath_, string fragmentPath_){
	filePathVertex = vertexPath_;
	filePathGeometry = geometryPath_;
	filePathFragment = fragmentPath_;

	cout << "Load Shader: ";

	GLuint vShader = CreateShader(
		(GLchar*)(dirPathShader + vertexPath_).c_str(),
		GL_VERTEX_SHADER
	);

	cout << vertexPath_;

	GLuint fShader = CreateShader(
		(GLchar*)(dirPathShader + fragmentPath_).c_str(),
		GL_FRAGMENT_SHADER
	);

	cout << " / " << fragmentPath_;

	if (geometryPath_.length() > 0) {
		GLuint gShader = CreateShader(
			(GLchar*)(dirPathShader + geometryPath_).c_str(),
			GL_GEOMETRY_SHADER
		);

		cout << " / " << geometryPath_;

		CreateProgram(vShader, gShader, fShader);

		glDeleteShader(gShader);
	} else {
		CreateProgram(vShader, fShader);
	}
	
	cout << endl;

	glDeleteShader(vShader);	
	glDeleteShader(fShader);
}

Shader::Shader(string vertexPath_, string fragmentPath_){
	LoadProgram(vertexPath_, "", fragmentPath_);
}

Shader::Shader(string vertexPath_, string geometryPath_, string fragmentPath_){
	LoadProgram(vertexPath_, geometryPath_, fragmentPath_);
}

Shader::~Shader(){
	glDeleteProgram(this->shaderID);
}

string Shader::GetDirectoryVertex()
{
	return filePathVertex;
}

string Shader::GetDirectoryGeometry()
{
	return filePathGeometry;
}

string Shader::GetDirectoryFragment()
{
	return filePathFragment;
}

void Shader::Use(){
	glUseProgram(shaderID);
}

GLuint Shader::GetUniformLocation(GLchar* var_name){
	return glGetUniformLocation(shaderID, var_name);
}

void Shader::OnEndUse(){
}
