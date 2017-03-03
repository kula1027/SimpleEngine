#include "ShaderMaterial.h"



ShaderMaterial::ShaderMaterial(GLuint shaderId_, const char* filePath_){
	shaderId = shaderId_;
	/*filePath = new char[strlen(filePath_) + 1];
	strcpy_s(filePath, strlen(filePath) + 1,filePath_);*/
}

ShaderMaterial::~ShaderMaterial(){
	delete(filePath);
}

char* ShaderMaterial::GetFilePath(){
	return filePath;
}

int ShaderMaterial::GetID(){
	return shaderId;
}
