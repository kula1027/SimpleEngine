#include "FileLoader.h"


Shader* FileLoader::LoadShader(string filePathVertex, string filePathFragment){
	string pathVertex = dirPathMaterial + filePathVertex;
	string pathFragment = dirPathMaterial + filePathFragment;

	Shader* shader = new Shader((GLchar*)pathVertex.c_str(), (GLchar*)pathFragment.c_str());
	
	return shader;
}

Shader * FileLoader::LoadShader(){
	return LoadShader("defaultVertex.vsh", "defaultFragment.fsh");
}

MeshModel* FileLoader::LoadMeshModel(string filePath){
	string path = dirPathMaterial + filePath;
	return new MeshModel((GLchar*)path.c_str());
}
