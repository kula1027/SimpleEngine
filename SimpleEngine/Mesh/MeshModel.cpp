#include "MeshModel.h"

#include <fstream>
#include <sstream>

#include "../Render/Texture.h"
#include "../FilePooler.h"
#include <Render/RenderMaterial/RenderMaterial.h>

MeshModel::MeshModel(){}

MeshModel::MeshModel(string path){
	this->meshes = new vector<Mesh*>();
	this->LoadModel(path);
}

MeshModel::~MeshModel(){
	for (int loop = 0; loop < meshes->size(); loop++) {
		delete(meshes->at(loop));
	}

	delete(this->meshes);
}

string MeshModel::GetFilePath(){
	return dirPath + fileName;
}

void MeshModel::LoadModel(string path){
	size_t foundIdx = path.find_last_of('/');
	this->dirPath = path.substr(0, foundIdx + 1);
	this->fileName = path.substr(foundIdx + 1);

	string fullFilePath = DirPathMaterial + dirPath + fileName;	

	// Read file via ASSIMP
	Assimp::Importer importer;
	std::cout << "Load Model... " + fullFilePath << endl;
	const aiScene* scene = importer.ReadFile(fullFilePath, aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_GenSmoothNormals | aiProcess_JoinIdenticalVertices);

	// Check for errors
	if (!scene || scene->mFlags == AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode){ // if is Not Zero
		cout << "ERROR::ASSIMP:: " << importer.GetErrorString() << endl;
		return;
	}
	std::cout << "\tMesh Count: " << scene->mNumMeshes << endl;
	
	// Process ASSIMP's root node recursively
	this->ProcessNode(scene->mRootNode, scene);

	int vCount = 0;
	int tCount = 0;
	for (int loop = 0; loop < meshes->size(); loop++) {
		vCount += meshes->at(loop)->vertices.size();
		tCount += meshes->at(loop)->triangles.size();
	}

	std::cout << "\tVertex Count: " << vCount
			  << " / Triangle Count: " << tCount << std::endl;
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
		vertex.position = vector;
		// normals
		vector.x = mesh->mNormals[i].x;
		vector.y = mesh->mNormals[i].y;
		vector.z = mesh->mNormals[i].z;
		vertex.normal = vector;
		// Texture Coordinates
		if (mesh->mTextureCoords[0]) // Does the mesh contain texture coordinates?
		{
			glm::vec2 vec;
			// A vertex can contain up to 8 different texture coordinates. We thus make the assumption that we won't 
			// use models where a vertex can have multiple texture coordinates so we always take the first set (0).
			vec.x = mesh->mTextureCoords[0][i].x;
			vec.y = mesh->mTextureCoords[0][i].y;
			vertex.texCoords = vec;
		}
		else
			vertex.texCoords = glm::vec2(0.0f, 0.0f);
		vertices.push_back(vertex);
	}
	// Now wak through each of the mesh's faces (a face is a mesh its triangle) and retrieve the corresponding vertex indices.
	for (GLuint i = 0; i < mesh->mNumFaces; i++)
	{
		aiFace face = mesh->mFaces[i];
		// Retrieve all indices of the face and store them in the indices vector
		for (GLuint j = 0; j < face.mNumIndices / 3; j++) {
			Triangle tri;
			for (int loop = 0; loop < 3; loop++) {				
				tri.idx[loop] = face.mIndices[j * 3 + loop];				
			}
			
			triangles.push_back(tri);
		}
	}	
	
	
	// Process materials
	RenderMaterial* renderMaterial = new RenderMaterial();
	if (mesh->mMaterialIndex >= 0)
	{
		aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];		

		renderMaterial->texDiffuse = this->LoadMaterialTextures(material, aiTextureType_DIFFUSE);
		renderMaterial->texSpec = this->LoadMaterialTextures(material, aiTextureType_SPECULAR);
		renderMaterial->texNormal = this->LoadMaterialTextures(material, aiTextureType_NORMALS);
	}

	return new Mesh(vertices, triangles, renderMaterial);
}

Texture * MeshModel::LoadMaterialTextures(aiMaterial * aiMat_, aiTextureType aiType_) {
	if (aiMat_->GetTextureCount(aiType_) > 0) {
		aiString strPath;
		aiMat_->GetTexture(aiType_, 0, &strPath);

		string fileNameTexture = string(strPath.C_Str());

		return FilePooler::LoadTexture(DirPathMaterial + dirPath + fileNameTexture);
	} else {
		return NULL;
	}		
}
