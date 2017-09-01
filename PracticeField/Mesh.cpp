#include "Mesh.h"

#include "Shader.h"

Mesh::Mesh() {}

Mesh::Mesh(vector<Vertex> vertices, vector<Triangle> triangles, vector<Texture> textures){
	this->vertices = vertices;
	this->triangles = triangles;
	this->textures = textures;

	this->SetupMesh();
}

int CompareVertex(const void* a, const void* b) {
	Vertex* vA = (Vertex*)a;
	Vertex* vB = (Vertex*)b;

	if (vA->dotValue > vB->dotValue) {
		return 1;
	}else if (vA->dotValue < vB->dotValue) {
		return -1;
	}else {
		return 0;
	}	
}

void Mesh::SetupMesh(){
	glGenVertexArrays(1, &this->VAO);
	glBindVertexArray(this->VAO);

	glGenBuffers(1, &this->VBO);
	glBindBuffer(GL_ARRAY_BUFFER, this->VBO);
	glBufferData(GL_ARRAY_BUFFER, this->vertices.size() * sizeof(Vertex),
		&this->vertices[0], GL_STATIC_DRAW);
	// Vertex Positions
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex),(GLvoid*)0);
	// Vertex Normals
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, Normal));
	// Vertex Texture Coords
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, TexCoords));

	glGenBuffers(1, &this->EBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, this->triangles.size() * sizeof(Triangle),
		&this->triangles[0], GL_STATIC_DRAW);
	
	glBindVertexArray(0);
}
