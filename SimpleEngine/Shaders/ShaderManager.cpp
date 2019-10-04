#include "ShaderManager.h"
#include "ShaderBundle.h"


ShaderManager* ShaderManager::instance;

ShaderManager::ShaderManager() {
}


ShaderManager::~ShaderManager() {
}

ShaderManager * ShaderManager::Inst() {
	if (instance == NULL) {
		instance = new ShaderManager();
	}

	return instance;
}

BaseShader* ShaderManager::GetShader(std::string filePath_) {
	BaseShader* retShader;

	if (shaderMap.count(filePath_) > 0) {
		retShader = shaderMap[filePath_];	
	} else {
		if (filePath_.compare("Forward/forward") == 0) {
			retShader = new ShaderForward();
			retShader->Initialize();
		}

		shaderMap.insert(make_pair(filePath_, retShader));
	}	

	return retShader;
}
