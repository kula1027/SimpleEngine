#include "MeshModel.h"

#include <fstream>
#include <sstream>

#include "Texture.h"
#include "FileManager.h"

MeshModel::MeshModel(){}

MeshModel::MeshModel(GLchar * path){
	this->meshes = new vector<Mesh*>();
	this->LoadModel(path);
}

MeshModel::~MeshModel(){
	for (int loop = 0; loop < meshes->size(); loop++) {
		delete(meshes->at(loop));
	}
	delete(this->meshes);
}

string MeshModel::GetDirectory(){
	return directory + fileName;
}


void MeshModel::LoadModel(string path){
	// Read file via ASSIMP
	Assimp::Importer importer;
	const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_GenNormals);
	std::cout << "Load Model... " + path << std::endl
		<< "Mesh Count: " << scene->mNumMeshes << std::endl;

	// Check for errors
	if (!scene || scene->mFlags == AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) // if is Not Zero
	{
		cout << "ERROR::ASSIMP:: " << importer.GetErrorString() << endl;
		return;
	}
	// Retrieve the directory path of the filepath
	this->directory = path.substr(0, path.find_last_of('/') + 1);
	this->fileName = path.substr(path.find_last_of('/') + 1, path.size());

	// Process ASSIMP's root node recursively
	this->ProcessNode(scene->mRootNode, scene);

	

	cout << endl;
}

void MeshModel::ProcessNode(aiNode * node, const aiScene * scene){
	// Process all the node's meshes (if any)
	for (GLuint i = 0; i < node->mNumMeshes; i++) {
		aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
		this->meshes->push_back(this->ProcessMesh(mesh, scene));
	}
	// Then do the same for each of its children
	for (GLuint i = 0; i < node->mNumChildren; i++){
		this->ProcessNode(node->mChildren[i], scene);
	}
}

Mesh* MeshModel::ProcessMesh(aiMesh * mesh, const aiScene * scene){
	// Data to fill
	vector<Vertex> vertices;
	//vector<GLuint> indices;
	vector<Triangle> triangles;
	vector<Texture*> textures;

	// Walk through each of the mesh's vertices
	for (GLuint i = 0; i < mesh->mNumVertices; i++)
	{
		Vertex vertex;
		glm::vec3 vector; // We declare a placeholder vector since assimp uses its own vector class that doesn't directly convert to glm's vec3 class so we transfer the data to this placeholder glm::vec3 first.
						  // Positions
		vector.x = mesh->mVertices[i].x;
		vector.y = mesh->mVertices[i].y;
		vector.z = mesh->mVertices[i].z;
		vertex.Position = vector;
		// Normals
		vector.x = mesh->mNormals[i].x;
		vector.y = mesh->mNormals[i].y;
		vector.z = mesh->mNormals[i].z;
		vertex.Normal = vector;
		// Texture Coordinates
		if (mesh->mTextureCoords[0]) // Does the mesh contain texture coordinates?
		{
			glm::vec2 vec;
			// A vertex can contain up to 8 different texture coordinates. We thus make the assumption that we won't 
			// use models where a vertex can have multiple texture coordinates so we always take the first set (0).
			vec.x = mesh->mTextureCoords[0][i].x;
			vec.y = mesh->mTextureCoords[0][i].y;
			vertex.TexCoords = vec;
		}
		else
			vertex.TexCoords = glm::vec2(0.0f, 0.0f);
		vertices.push_back(vertex);
	}
	// Now wak through each of the mesh's faces (a face is a mesh its triangle) and retrieve the corresponding vertex indices.
	for (GLuint i = 0; i < mesh->mNumFaces; i++)
	{
		aiFace face = mesh->mFaces[i];
		// Retrieve all indices of the face and store them in the indices vector
		for (GLuint j = 0; j < face.mNumIndices / 3; j++) {
			Triangle tri;
			tri.idx0 = face.mIndices[j * 3];
			tri.idx1 = face.mIndices[j * 3 + 1];
			tri.idx2 = face.mIndices[j * 3 + 2];
			triangles.push_back(tri);
		}
	}	
	
	// Process materials
	if (mesh->mMaterialIndex >= 0)
	{
		aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
		// We assume a convention for sampler names in the shaders. Each diffuse texture should be named
		// as 'texture_diffuseN' where N is a sequential number ranging from 1 to MAX_SAMPLER_NUMBER. 
		// Same applies to other texture as the following list summarizes:
		// Diffuse: texture_diffuseN
		// Specular: texture_specularN
		// Normal: texture_normalN		
		

		// 1. Diffuse maps
		vector<Texture*> diffuseMaps = this->LoadMaterialTextures(material, aiTextureType_DIFFUSE);
		textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());
		// 2. Specular maps
		vector<Texture*> specularMaps = this->LoadMaterialTextures(material, aiTextureType_SPECULAR);
		textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());
		// 3. Normal maps
		vector<Texture*> normalMaps = this->LoadMaterialTextures(material, aiTextureType_NORMALS);		
		textures.insert(textures.end(), normalMaps.begin(), normalMaps.end());
	}

	// Return a mesh object created from the extracted mesh data
	return new Mesh(vertices, triangles, textures);
}

vector<Texture*> MeshModel::LoadMaterialTextures(aiMaterial * mat, aiTextureType type) {
	vector<Texture*> textures;
	
	for (GLuint i = 0; i < mat->GetTextureCount(type); i++)
	{	
		TextureType tType;
		switch (type) {
		case aiTextureType_DIFFUSE:
			tType = TextureType_Diffuse;
			break;
		case aiTextureType_SPECULAR:
			tType = TextureType_Specular;
			break;
		case aiTextureType_NORMALS:
			tType = TextureType_Normals;
			break;

		default:
			tType = TextureType_Diffuse;
			break;
		}

		aiString str;
		mat->GetTexture(type, i, &str);
		
		string filePath = string(str.C_Str());
		std::cout << filePath << std::endl;

		filePath = directory.substr(directory.find_first_of('/') + 1, directory.size()) + filePath;

		Texture* texture = FileManager::LoadTexture(filePath, tType);
		textures.push_back(texture);
	}

	return textures;
}