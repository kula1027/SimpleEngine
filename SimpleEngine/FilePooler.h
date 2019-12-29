#pragma once


#include <string>
#include <vector>
#include <map>
#include "Render/Texture.h"

class BaseShader;
class MeshModel;
class Texture;

static class FilePooler {
private:
	static std::map<std::string, MeshModel*> loadedMeshModels;
	static std::map<std::string, Texture*> loadedTextures;

public:

	static Texture* LoadTexture(std::string filePath);
	static MeshModel* LoadMeshModel(std::string filePath);
};