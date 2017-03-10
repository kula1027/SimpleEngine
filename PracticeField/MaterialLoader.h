#pragma once

#define dirPathMaterial "Materials/"

#include <vector>

#include "Shader.h"
#include "MeshModel.h"

static class MaterialLoader
{
private :

	static std::vector<Shader*> shaders;
	static std::vector<MeshModel*> meshModels;
		
public:
	static Shader* LoadShader(string filePathVertex, string filePathFragment);
	static MeshModel* LoadMeshModel(string filePath);
};

