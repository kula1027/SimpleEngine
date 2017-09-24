#pragma once

// Std. Includes
#include <string>
#include <iostream>
#include <map>
#include <vector>

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

#include "Mesh.h"


using namespace std;

class Texture;

class MeshModel
{
public:
	/*  Functions   */
	MeshModel();
	MeshModel(GLchar* path);
	~MeshModel();
	MeshModel* CopyMesh();
	vector<Mesh*>* meshes;

	string GetDirectory();

protected:

private:
	/*  Model Data  */
	string directory;	
	string fileName;

	void ProcessNode(aiNode* node, const aiScene* scene);
	Mesh* ProcessMesh(aiMesh* mesh, const aiScene* scene);
	void LoadModel(string path);
	vector<Texture*> LoadMaterialTextures(aiMaterial* mat, aiTextureType type);
};

