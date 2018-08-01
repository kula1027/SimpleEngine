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
	static std::vector<BaseShader*> loadedShaders;
	static std::vector<Texture*> loadedTextures;

public:
	static BaseShader* LoadShader(std::string filePathVertex, std::string filePathFragment);
	static BaseShader* LoadShader(std::string filePathVertex, std::string filePathGeometry, std::string filePathFragment);

	static Texture* LoadTexture(std::string filePath, TextureType type_);//TODO: 로드할때 텍스쳐 타입 필요없음
	static MeshModel* LoadMeshModel(std::string filePath);		

	static void InitializeShaders();
};

