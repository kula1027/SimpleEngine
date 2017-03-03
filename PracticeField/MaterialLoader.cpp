#include "MaterialLoader.h"

std::vector<ShaderMaterial*> MaterialLoader::shaders;
std::vector<TextureMaterial*> MaterialLoader::textures;
std::vector<MeshMaterial*> MaterialLoader::meshes;


ShaderMaterial* MaterialLoader::LoadShader(const char* filePath){
	int vectorCount = shaders.size();
	for (int loop = 0; loop < vectorCount; loop++) {
		if (strcmp(filePath, shaders.at(loop)->GetFilePath())) {
			return shaders.at(loop);
		}
	}

/*	int strLength = strlen(dirPathMaterial) + strlen(filePath) + 1;
	char* path = new char[strLength];
	strcpy_s(path, strLength, dirPathMaterial);
	strcat_s(path, strLength, filePath);*/

	ShaderMaterial* shaderMaterial = new ShaderMaterial(LoadShaders("Materials/SimpleShader.shader"), filePath);
	shaders.push_back(shaderMaterial);
	
	return shaderMaterial;
}

TextureMaterial* MaterialLoader::LoadTexture(const char* filePath){
	/*int vectorCount = textures.size();
	for (int loop = 0; loop < vectorCount; loop++) {
		if (strcmp(filePath, textures.at(loop)->GetFilePath())) {
			return textures.at(loop);
		}
	}

	ShaderMaterial* shaderMaterial = new ShaderMaterial(LoadShaders("Materials/SimpleShader.shader"), filePath);
	shaders.push_back(shaderMaterial);
	loadDDS(filePath);*/
	return NULL;
}

MeshMaterial* MaterialLoader::LoadMesh(const char* filePath){
	//loadOBJ(filePath);
	return NULL;
}
