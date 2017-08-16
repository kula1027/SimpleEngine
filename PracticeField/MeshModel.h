#pragma once

// Std. Includes
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <map>
#include <vector>
using namespace std;

#include <GL\glew.h>
#include <GLFW\glfw3.h>

#include "Shader.h"
#include "Camera.h"

// GLM Mathemtics
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <assimp\postprocess.h>
#include <assimp\Importer.hpp>
#include <assimp\scene.h>

#include <SOIL\SOIL.h>
#include "Mesh.h"


using namespace std;

class MeshModel
{
public:
	/*  Functions   */
	MeshModel(GLchar* path);
	~MeshModel();
	vector<Mesh> GetMeshes();
	vector<Mesh> meshes;

	string GetDirectory();

private:
	/*  Model Data  */
	
	string directory;
	vector<Texture> textures_loaded;	
	// Stores all the textures loaded so far, optimization to make sure textures aren't loaded more than once.
	/*  Functions   */
	void LoadModel(string path);
	void ProcessNode(aiNode* node, const aiScene* scene);
	Mesh ProcessMesh(aiMesh* mesh, const aiScene* scene);
	vector<Texture> LoadMaterialTextures(aiMaterial* mat, aiTextureType type, string typeName);

	GLint TextureFromFile(const char* path, string directory);
};

