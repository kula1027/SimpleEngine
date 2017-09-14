#include "FileManager.h"

#include "Shader.h"
#include "MeshModel.h"

std::vector<MeshModel*> FileManager::loadedMeshModels;
std::vector<Shader*> FileManager::loadedShaders;
std::vector<Texture*> FileManager::loadedTextures;

Shader* FileManager::LoadShader(string filePathVertex, string filePathFragment){
	string pathVertex = (string)dirPathMaterial + (string)dirPathShader + filePathVertex;
	string pathFragment = (string)dirPathMaterial + (string)dirPathShader + filePathFragment;

	Shader* shader = new Shader((GLchar*)pathVertex.c_str(), (GLchar*)pathFragment.c_str());
	
	return shader;
}

Shader * FileManager::LoadShader(){
	return LoadShader(
		"defaultVertex.vert", 
		"defaultFragment.frag"
	);
}

Texture * FileManager::LoadTexture(std::string filePath, TextureType type_){
	string path = dirPathMaterial + filePath;

	Texture* foundTexture = NULL;
	for (int loop = 0; loop < loadedTextures.size(); loop++) {
		if (loadedTextures[loop]->GetDirectory().compare(path.c_str()) == 0) {
			foundTexture = loadedTextures[loop];
			break;
		}
	}

	if (foundTexture != NULL) {
		return foundTexture;
	}
	else {
		Texture* texture = new Texture(path, type_);
		loadedTextures.push_back(texture);

		return texture;
	}
}

MeshModel* FileManager::LoadMeshModel(string filePath) {
	string path = dirPathMaterial + filePath;

	MeshModel* foundMesh = NULL;
	for (int loop = 0; loop < loadedMeshModels.size(); loop++) {
		if (loadedMeshModels[loop]->GetDirectory().compare(path.c_str()) == 0) {
			foundMesh = loadedMeshModels[loop];
			break;
		}
	}

	if (foundMesh != NULL) {
		return foundMesh;
	}
	else {
		MeshModel* meshModel = new MeshModel((GLchar*)path.c_str());
		loadedMeshModels.push_back(meshModel);

		return meshModel;
	}
}