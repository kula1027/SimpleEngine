#include "ShaderManager.h"
#include "ShaderBundle.h"


std::map<std::string, BaseShader*> ShaderManager::loadedShaders;

ShaderManager::ShaderManager() {
}


ShaderManager::~ShaderManager() {
}

BaseShader* ShaderManager::GetShader(std::string filePath_) {
	BaseShader* retShader;

	if (loadedShaders.count(filePath_) > 0) {
		retShader = loadedShaders[filePath_];	
	} else {
		if (filePath_.compare("Forward/forward") == 0) {			
			retShader = new ShaderForward();				
		}

		loadedShaders.insert(make_pair(filePath_, retShader));
	}	

	return retShader;
}
