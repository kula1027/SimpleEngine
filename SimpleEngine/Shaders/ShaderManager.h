#pragma once
#include <map>
#include <string>


class BaseShader;
class ShaderManager {
private:
	ShaderManager();
	static ShaderManager* instance;

	std::map<std::string, BaseShader*> shaderMap;

public:
	~ShaderManager();

	static ShaderManager* Inst();
	
	BaseShader* GetShader(std::string shaderName_);
};

