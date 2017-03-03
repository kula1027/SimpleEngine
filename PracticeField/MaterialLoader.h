#pragma once

#define dirPathMaterial "Materials/"

#include <vector>

#include "ObjLoader.h"
#include "TextureLoader.h"
#include "ShaderLoader.h"

#include "MeshMaterial.h"
#include "TextureMaterial.h"
#include "ShaderMaterial.h"

static class MaterialLoader
{
private :

	static std::vector<ShaderMaterial*> shaders;
	static std::vector<TextureMaterial*> textures;
	static std::vector<MeshMaterial*> meshes;
		
public:
	static ShaderMaterial* LoadShader(const char* name);
	static TextureMaterial* LoadTexture(const char* name);
	static MeshMaterial* LoadMesh(const char* name);
};

