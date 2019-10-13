#include "FilePooler.h"

#include <Shaders/BaseShader.h>
#include "Mesh/MeshModel.h"
#include <Debugger/SP_Debugger.h>

std::map<std::string, MeshModel*>  FilePooler::loadedMeshModels;
std::map<std::string, Texture*> FilePooler::loadedTextures;


Texture * FilePooler::LoadTexture(std::string filePath){	
	Texture* retTexture = NULL;

	if (loadedTextures.find(filePath) == loadedTextures.end()) {//not found
		Texture* texture = new Texture(filePath);
		loadedTextures.insert(make_pair(filePath, texture));

		retTexture = texture;
	} else {
		retTexture = loadedTextures[filePath];
	}

	return retTexture;
}

MeshModel* FilePooler::LoadMeshModel(string filePath) {
	MeshModel* retMeshModel = NULL;

	if (loadedMeshModels.find(filePath) == loadedMeshModels.end()) {//not found
		MeshModel* meshModel = new MeshModel(filePath);
		loadedMeshModels.insert(make_pair(filePath, meshModel));		

		retMeshModel = meshModel;
	} else {
		retMeshModel = loadedMeshModels[filePath];
	}

	return retMeshModel;
}
