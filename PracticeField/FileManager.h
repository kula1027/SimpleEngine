#pragma once

#define dirPathMaterial "Materials/"
#define dirPathShader "Shaders/"

#include <string>
#include <vector>
#include "Render/Texture.h"

class Shader;
class MeshModel;
class Texture;

static class FileManager
{
private :
	static std::vector<MeshModel*> loadedMeshModels;
	static std::vector<Shader*> loadedShaders;
	static std::vector<Texture*> loadedTextures;

public:
	static Shader* LoadShader(std::string filePathVertex, std::string filePathFragment);
	static Shader* LoadShader(std::string filePathVertex, std::string filePathGeometry, std::string filePathFragment);
	static Texture* LoadTexture(std::string filePath, TextureType type_);
	static MeshModel* LoadMeshModel_Pool(std::string filePath);	
	static MeshModel* LoadMeshModel(std::string filePath);
};

