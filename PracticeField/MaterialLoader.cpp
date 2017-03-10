#include "MaterialLoader.h"

std::vector<Shader*> MaterialLoader::shaders;
std::vector<MeshModel*> MaterialLoader::meshModels;


Shader* MaterialLoader::LoadShader(string filePathVertex, string filePathFragment){
	string pathVertex = dirPathMaterial + filePathVertex;
	string pathFragment = dirPathMaterial + filePathFragment;

	Shader* shader = new Shader((GLchar*)pathVertex.c_str(), (GLchar*)pathFragment.c_str());
	shaders.push_back(shader);
	
	return shader;
}

MeshModel* MaterialLoader::LoadMeshModel(string filePath){
	string path = dirPathMaterial + filePath;
	return new MeshModel((GLchar*)path.c_str());
}
