#pragma once
#include <map>
#include <string>


class BaseShader;
class ShaderManager {
private:
	ShaderManager();	

	static std::map<std::string, BaseShader*> loadedShaders;

public:
	~ShaderManager();	
	
	static BaseShader* GetShader(std::string shaderName_);
};

