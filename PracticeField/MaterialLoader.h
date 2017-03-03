#pragma once
#include <vector>
#include "MeshMaterial.h"
#include "TextureMaterial.h"
#include "ShaderMaterial.h"

#include "ObjLoader.h"
#include "TextureLoader.h"
#include "ShaderLoader.h"

static class MaterialLoader
{
private :
	const char* dirPathMaterial = "Materials";

	std::vector<ShaderMaterial*> shaders;
	std::vector<TextureMaterial*> textures;
	std::vector<MeshMaterial*> meshes;
		
public:
	ShaderMaterial* LoadShader(const char* name);
	TextureMaterial* LoadTexture(const char* name);
	MeshMaterial* LoadMesh(const char* name);
};

