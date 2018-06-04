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

Shader::Shader(GLchar* vertexPath_, GLchar* fragmentPath_){
	GLuint vShader = CreateShader(vertexPath_, GL_VERTEX_SHADER);
	GLuint fShader = CreateShader(fragmentPath_, GL_FRAGMENT_SHADER);

	CreateProgram(vShader, fShader);

	glDeleteShader(vShader);
	glDeleteShader(fShader);
}

Shader::Shader(GLchar* vertexPath_, GLchar* geometryPath_, GLchar* fragmentPath_){
	GLuint vShader = CreateShader(vertexPath_, GL_VERTEX_SHADER);
	GLuint gShader = CreateShader(geometryPath_, GL_GEOMETRY_SHADER);
	GLuint fShader = CreateShader(fragmentPath_, GL_FRAGMENT_SHADER);
	
	CreateProgram(vShader, gShader, fShader);

	glDeleteShader(vShader);
	glDeleteShader(gShader);
	glDeleteShader(fShader);
}

Shader::~Shader(){
	glDeleteProgram(this->shaderID);
}

char* Shader::GetFilePath(){
	return "";
}

void Shader::Use(){
	glUseProgram(shaderID);
}

GLuint Shader::GetUniformLocation(GLchar* var_name){
	return glGetUniformLocation(shaderID, var_name);
}

void Shader::OnEndUse(){
}
