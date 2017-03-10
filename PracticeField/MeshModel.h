#pragma once

// Std. Includes
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <map>
#include <vector>
using namespace std;

// GLEW
#include <GL\glew.h>

// GLFW
#include <GLFW\glfw3.h>

// GL includes
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
	void Draw(Shader* shader);
private:
	/*  Model Data  */
	vector<Mesh> meshes;
	string directory;
	vector<Texture> textures_loaded;	
	// Stores all the textures loaded so far, optimization to make sure textures aren't loaded more than once.
	/*  Functions   */
	void loadModel(string path);
	void processNode(aiNode* node, const aiScene* scene);
	Mesh processMesh(aiMesh* mesh, const aiScene* scene);
	vector<Texture> loadMaterialTextures(aiMaterial* mat, aiTextureType type, string typeName);

	GLint TextureFromFile(const char* path, string directory);
};

