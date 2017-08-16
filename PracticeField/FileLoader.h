#pragma once

#define dirPathMaterial "Materials/"

#include "Shader.h"
#include "MeshModel.h"

static class FileLoader
{
private :
		
public:
	static Shader* LoadShader(string filePathVertex, string filePathFragment);
	static Shader* LoadShader();
	static MeshModel* LoadMeshModel(string filePath);
};

