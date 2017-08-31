#include "FileLoader.h"

#include "Shader.h"
#include "MeshModel.h"

std::vector<MeshModel*> FileLoader::loadedMeshModels;
std::vector<Shader*> FileLoader::loadedShaders;

Shader* FileLoader::LoadShader(string filePathVertex, string filePathFragment){
	string pathVertex = (string)dirPathMaterial + (string)dirPathShader + filePathVertex;
	string pathFragment = (string)dirPathMaterial + (string)dirPathShader + filePathFragment;

	Shader* shader = new Shader((GLchar*)pathVertex.c_str(), (GLchar*)pathFragment.c_str());
	
	return shader;
}

Shader * FileLoader::LoadShader(){
	return LoadShader(
		"defaultVertex.vert", 
		"defaultFragment.frag"
	);
}

MeshModel* FileLoader::LoadMeshModel(string filePath) {
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