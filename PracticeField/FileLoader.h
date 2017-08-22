#pragma once

#define dirPathMaterial "Materials/"

#include <string>
#include <vector>

class Shader;
class MeshModel;

static class FileLoader
{
private :
	static std::vector<MeshModel*> loadedMeshModels;
	static std::vector<Shader*> loadedShaders;

public:
	static Shader* LoadShader(std::string filePathVertex, std::string filePathFragment);
	static Shader* LoadShader();
	static MeshModel* LoadMeshModel(std::string filePath);
};

