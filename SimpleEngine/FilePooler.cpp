#include "FilePooler.h"

#include <Shaders/BaseShader.h>
#include "Mesh/MeshModel.h"
#include <Debugger/SP_Debugger.h>

std::vector<MeshModel*> FilePooler::loadedMeshModels;
std::vector<Texture*> FilePooler::loadedTextures;



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
