#pragma once


#include <string>
#include <vector>
#include "Render/Texture.h"

class Shader;
class MeshModel;
class Texture;

static class FilePooler
{
private :
	static std::vector<MeshModel*> loadedMeshModels;
	static std::vector<Shader*> loadedShaders;
	static std::vector<Texture*> loadedTextures;

public:
	static Shader* LoadShader(std::string filePathVertex, std::string filePathFragment);
	static Shader* LoadShader(std::string filePathVertex, std::string filePathGeometry, std::string filePathFragment);
	static Texture* LoadTexture(std::string filePath, TextureType type_);//TODO: 로드할때 텍스쳐 타입 필요없음
	static MeshModel* LoadMeshModel(std::string filePath);		
};

