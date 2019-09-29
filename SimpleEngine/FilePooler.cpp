#include "FilePooler.h"

#include <Shaders/BaseShader.h>
#include "Mesh/MeshModel.h"
#include <Debugger/SP_Debugger.h>

std::vector<MeshModel*> FilePooler::loadedMeshModels;
std::vector<BaseShader*> FilePooler::loadedShaders;
std::vector<Texture*> FilePooler::loadedTextures;

BaseShader* FilePooler::LoadShader(string filePathVertex, string filePathFragment){
	return LoadShader(filePathVertex, "", filePathFragment);
}

//????
BaseShader * FilePooler::LoadShader(std::string filePathVertex, std::string filePathGeometry, std::string filePathFragment){
	BaseShader* retShader = NULL;

	for (int loop = 0; loop < loadedShaders.size(); loop++) {
		int hit = 0;
		if (loadedShaders[loop]->GetDirectoryVertex().compare(filePathVertex) == 0) {
			hit++;			
		}
		if (loadedShaders[loop]->GetDirectoryGeometry().compare(filePathGeometry) == 0) {
			hit++;
		}
		if (loadedShaders[loop]->GetDirectoryFragment().compare(filePathFragment) == 0) {
			hit++;
		}

		if (hit == 3) {
			retShader = loadedShaders[loop];
			break;
		}
	}

	if (retShader == NULL) {
		SP_Debugger::Log("Load Shader: ");
		SP_Debugger::Log("\t V : " + filePathVertex);
		SP_Debugger::Log("\t G : " + filePathGeometry);
		SP_Debugger::Log("\t F : " + filePathFragment);

		retShader = BaseShader::GetShader(filePathVertex, filePathGeometry, filePathFragment);

		if (retShader == NULL) {
			DebugError("Matching Shader not found");		
			DebugError("\t" + filePathVertex);
			DebugError("\t" + filePathFragment);
		} else {
			loadedShaders.push_back(retShader);
		}		
	}

	retShader->Initialize();

	return retShader;
}


Texture * FilePooler::LoadTexture(std::string filePath){
	std::cout << "\tLoad Texture... " << filePath << std::endl;	

	Texture* retTexture = NULL;
	for (int loop = 0; loop < loadedTextures.size(); loop++) {
		if (loadedTextures[loop]->GetDirectory().compare(filePath.c_str()) == 0) {
			retTexture = loadedTextures[loop];
			break;
		}
	}

	if (retTexture == NULL) {
		retTexture = new Texture(filePath);
		loadedTextures.push_back(retTexture);
	} 

	return retTexture;
}

MeshModel* FilePooler::LoadMeshModel(string filePath) {
	MeshModel* foundMesh = NULL;
	for (int loop = 0; loop < loadedMeshModels.size(); loop++) {
		if (loadedMeshModels[loop]->GetFilePath().compare(filePath) == 0) {
			foundMesh = loadedMeshModels[loop];
			break;
		}
	}

	if (foundMesh != NULL) {
		return foundMesh;
	}
	else {
		MeshModel* meshModel = new MeshModel(filePath);
		loadedMeshModels.push_back(meshModel);

		return meshModel;
	}
}
