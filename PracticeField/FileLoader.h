#pragma once

#define dirPathMaterial "Materials/"

#include <string>

class Shader;
class MeshModel;

static class FileLoader
{
private :
		
public:
	static Shader* LoadShader(std::string filePathVertex, std::string filePathFragment);
	static Shader* LoadShader();
	static MeshModel* LoadMeshModel(std::string filePath);
};

