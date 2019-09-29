#include "BaseShader.h"

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

#include "ShaderBundle.h"

#include <glm/gtc/type_ptr.hpp>


#pragma region Init
std::string BaseShader::ReadCodeFromFile(std::string path_) {
	std::string strCode;
	std::ifstream fileStream;
	fileStream.exceptions(std::ifstream::badbit);

	try {
		fileStream.open(path_);
		std::stringstream strStream;
		strStream << fileStream.rdbuf();
		fileStream.close();
		strCode = strStream.str();
	} catch (std::ifstream::failure e) {
		std::cout << "Error _ ReadCodeFromFile _ " << path_ << std::endl;
	}

	return strCode;
}

void BaseShader::CompileCode(int shaderId_, const GLchar* code_) {
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

GLuint BaseShader::CreateShader(GLchar* path_, int shaderType_) {
	std::string code = ReadCodeFromFile(path_);
	const GLchar* chCode = code.c_str();

	GLuint shader;
	shader = glCreateShader(shaderType_);
	CompileCode(shader, chCode);

	return shader;
}

void BaseShader::CreateProgram(GLuint shader0, GLuint shader1, GLuint shader2) {
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
	if (!success) {
		glGetProgramInfoLog(this->shaderID, LogLength, NULL, infoLog);
		std::cout << "Error _ CreateProgram\n" << infoLog << std::endl;
	}
}

void BaseShader::CreateProgram(GLuint shader0, GLuint shader1) {
	GLint success;
	GLchar infoLog[LogLength];

	// Shader Program
	this->shaderID = glCreateProgram();
	glAttachShader(this->shaderID, shader0);
	glAttachShader(this->shaderID, shader1);
	glLinkProgram(this->shaderID);
	// Print linking errors if any
	glGetProgramiv(this->shaderID, GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(this->shaderID, LogLength, NULL, infoLog);
		std::cout << "Error _ CreateProgram\n" << infoLog << std::endl;
	}
}

void BaseShader::LoadProgram(string vertexPath_, string geometryPath_, string fragmentPath_) {
	GLuint vShader = CreateShader(
		(GLchar*)(dirPathShader + vertexPath_).c_str(),
		GL_VERTEX_SHADER
	);	

	GLuint fShader = CreateShader(
		(GLchar*)(dirPathShader + fragmentPath_).c_str(),
		GL_FRAGMENT_SHADER
	);

	
	if (geometryPath_.length() > 0) {
		GLuint gShader = CreateShader(
			(GLchar*)(dirPathShader + geometryPath_).c_str(),
			GL_GEOMETRY_SHADER
		);
		
		CreateProgram(vShader, gShader, fShader);

		glDeleteShader(gShader);
	} else {
		CreateProgram(vShader, fShader);
	}

	glDeleteShader(vShader);
	glDeleteShader(fShader);
}

BaseShader::BaseShader() {
}

BaseShader::BaseShader(std::string filePathVertex_, std::string filePathFragment_) {
	filePathVertex = filePathVertex_;
	filePathGeometry = "";
	filePathFragment = filePathFragment_;

	LoadProgram(filePathVertex, filePathGeometry, filePathFragment);
}

BaseShader::BaseShader(std::string filePathVertex_, std::string filePathGeometry_, std::string filePathFragment_) {
	filePathVertex = filePathVertex_;
	filePathGeometry = filePathGeometry_;
	filePathFragment = filePathFragment_;	
	
	LoadProgram(filePathVertex, filePathGeometry, filePathFragment);
}

//return matching shader instance
BaseShader * BaseShader::GetShader(std::string filePathVertex, std::string filePathGeometry, std::string filePathFragment) {
	/*if (filePathVertex.compare("default.vert") == 0 &&
		filePathGeometry.compare("") == 0 &&
		filePathFragment.compare("default.frag") == 0) {
		return new ShaderForward();
	}
	if (filePathVertex.compare("shadowMap.vert") == 0 &&
		filePathGeometry.compare("") == 0 &&
		filePathFragment.compare("shadowMap.frag") == 0) {
		return new BaseShader("shadowMap.vert", "shadowMap.frag");
	}
	if (filePathVertex.compare("trans_inst_nocull.vert") == 0 &&
		filePathGeometry.compare("") == 0 &&
		filePathFragment.compare("trans_inst_nocull.frag") == 0) {
		return new InstancedShader();
	}
	if (filePathVertex.compare("3layer.vert") == 0 &&
		filePathGeometry.compare("") == 0 &&
		filePathFragment.compare("3layer.frag") == 0) {
		return new ShaderForward("3layer.vert", "3layer.frag");
	}
	if (filePathVertex.compare("3layer.vert") == 0 &&
		filePathGeometry.compare("") == 0 &&
		filePathFragment.compare("3layer.frag") == 0) {
		return new ShaderForward("3layer.vert", "3layer.frag");
	}
	if (filePathVertex.compare("vertexColorDiffuse.vert") == 0 &&
		filePathGeometry.compare("") == 0 &&
		filePathFragment.compare("vertexColorDiffuse.frag") == 0) {
		return new ShaderForward("vertexColorDiffuse.vert", "vertexColorDiffuse.frag");
	}*/
	
	printf("No Matching Shaders Found.\n");

	return NULL;
}
#pragma endregion

BaseShader::~BaseShader(){
	glDeleteProgram(this->shaderID);
}

string BaseShader::GetDirectoryVertex() {
	return filePathVertex;
}

string BaseShader::GetDirectoryGeometry() {
	return filePathGeometry;
	glCreateProgram();
}

string BaseShader::GetDirectoryFragment() {
	return filePathFragment;
}

void BaseShader::Use(){
	glUseProgram(shaderID);
}

GLuint BaseShader::GetUniformLocation(const GLchar* var_name){
	return glGetUniformLocation(shaderID, var_name);
}

void BaseShader::SetMat4(string var_name, glm::mat4 mat4_){
	GLuint loc = GetUniformLocation(var_name.c_str());
	glUniformMatrix4fv(loc, 1, GL_FALSE, glm::value_ptr(mat4_));
}

void BaseShader::SetVec3(string var_name, glm::vec3 vec3_) {
	GLuint loc = GetUniformLocation(var_name.c_str());
	glUniform3f(loc, vec3_.x, vec3_.y, vec3_.z);
}

void BaseShader::SetInt(string var_name, int val_) {
	GLuint loc = GetUniformLocation(var_name.c_str());
	glUniform1i(loc, val_);
}


void BaseShader::OnEndUse(){
}
