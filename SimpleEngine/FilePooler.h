#pragma once


#include <string>
#include <vector>
#include "Render/Texture.h"

class BaseShader;
class MeshModel;
class Texture;

static class FilePooler
{
private :
	static std::vector<MeshModel*> loadedMeshModels;
	
	static std::vector<Texture*> loadedTextures;

public:
	
	static Texture* LoadTexture(std::string filePath);
	static MeshModel* LoadMeshModel(std::string filePath);		

	static void InitializeShaders();
};

