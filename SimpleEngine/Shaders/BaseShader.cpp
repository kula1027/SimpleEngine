#include "BaseShader.h"
#include "ShaderBundle.h"
#include "ShaderDef.h"
#include <algorithm>
#include <Debugger/SP_Debugger.h>
#include <EngineDef.h>
#include <fstream>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include <sstream>
#include <string>
#include <Utils.h>


#pragma region Init
int BaseShader::currentUsingShader = -1;

BaseShader::BaseShader() {
}

BaseShader::BaseShader(std::string filePath_) {
	filePath = filePath_;
	LoadProgram(filePath_);


	id_matrice.model = GetUniformLocation("M");
	id_matrice.mvp = GetUniformLocation("MVP");

	DebugLog("Shader Created: " + filePath);	
}


/// <summary>
/// #include 키워드를 해당 코드로 치환한다.
/// </summary>
std::string BaseShader::ParseIncludes(std::string strCode_, std::string path_) {
	std::string retCode = "";

	std::istringstream stream(strCode_);
	std::string line;
	while (std::getline(stream, line)){
		if (line.substr(0, 4).compare("//! ") == 0) {// "//! "로 시작하는 줄?
			stringstream lineStream(line);
			string strTokenized;

			int tokenCount = 0;
			while (std::getline(lineStream, strTokenized, ' ')) {											
				if (tokenCount == 2) {					
					strTokenized.erase(std::remove(strTokenized.begin(), strTokenized.end(), '"'), strTokenized.end());
					std::string pathCode = Utils::TravelPath(path_, strTokenized);
					std::string includeCode = ReadCodeFromFile(pathCode);															

					retCode += includeCode + "\n";					
					break;
				}
				tokenCount++;
			}
		} else {			
			retCode += line + "\n";
		}
	}		

	return retCode;
}

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

	strCode = ParseIncludes(strCode, path_);

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

/// <summary>
/// 
/// </summary>
/// <param name="path_"></param>
/// <param name="shaderType_">eg. VS, GEO, FRAG shader</param>
/// <returns></returns>
GLuint BaseShader::CreateShader(GLchar* path_, int shaderType_) {
	std::string strCode = ReadCodeFromFile(path_);
	if (strCode.size() == 0) {
		std::cout << "Shader is Empty\n";
		return -1;
	}			

	GLuint shader;
	shader = glCreateShader(shaderType_);
	CompileCode(shader, strCode.c_str());

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

void BaseShader::LoadProgram(string filePath_) {
	GLuint vShader = CreateShader(
		(GLchar*)(DirPathShader + filePath_ + VertexShaderFormat).c_str(),
		GL_VERTEX_SHADER
	);

	GLuint fShader = CreateShader(
		(GLchar*)(DirPathShader + filePath_ + FragmentShaderFormat).c_str(),
		GL_FRAGMENT_SHADER
	);

	GLuint gShader = CreateShader(
		(GLchar*)(DirPathShader + filePath_ + GeometryShaderFormat).c_str(),
		GL_GEOMETRY_SHADER
	);

	if (gShader == -1) {
		CreateProgram(vShader, fShader);
	} else {
		CreateProgram(vShader, gShader, fShader);
	}

	glDeleteShader(vShader);
	glDeleteShader(fShader);
}



void BaseShader::BindLightUBO() {
	unsigned int lights_index = glGetUniformBlockIndex(shaderID, "LightData");
	glUniformBlockBinding(shaderID, lights_index, BindingPointLightData);
}

void BaseShader::BindCameraUBO() {
	unsigned int camera_idx = glGetUniformBlockIndex(shaderID, "CameraData");
	glUniformBlockBinding(shaderID, camera_idx, BindingPointCameraData);
}

#pragma endregion

BaseShader::~BaseShader(){
	glDeleteProgram(this->shaderID);
}

string BaseShader::GetFilePath() {
	return filePath;
}

void BaseShader::Use(){
	if(currentUsingShader != shaderID)
		glUseProgram(shaderID);	
}

GLuint BaseShader::GetUniformLocation(const GLchar* var_name){
	return glGetUniformLocation(shaderID, var_name);
}

GLuint BaseShader::GetUniformBlockIndex(const GLchar * var_name) {
	return  glGetUniformBlockIndex(shaderID, var_name);
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

void BaseShader::SetMat_M(glm::mat4 matM_) {
	glUniformMatrix4fv(id_matrice.model, 1, GL_FALSE, glm::value_ptr(matM_));
}

void BaseShader::SetMat_MVP(glm::mat4 matMVP_) {
	glUniformMatrix4fv(id_matrice.mvp, 1, GL_FALSE, glm::value_ptr(matMVP_));
}