#include "Mesh.h"

#include "Shader.h"
#include "Texture.h"

#include <fstream>
#include <sstream>

Mesh::Mesh() {}

Mesh::Mesh(vector<Vertex> vertices_, vector<Triangle> triangles_, vector<Texture*> textures_){
	this->vertices = vertices_;
	this->triangles = triangles_;
	this->textures = textures_;
}
