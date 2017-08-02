#include "Mesh.h"

Mesh::Mesh(vector<Vertex> vertices, vector<Triangle> triangles, vector<Texture> textures){
	this->vertices = vertices;
	this->triangles = triangles;
	this->textures = textures;

	RearrangeFace();
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

void SortTriangles(vector<Vertex>* vertices_, vector<Triangle>* triangles_, vector<float>* dotValues, int lo, int hi) {
	int i = lo, j = hi;
	Triangle tempTri;	
	float tempDot;
	float pivot = (*dotValues)[(lo + hi) / 2];

	do {
		while ((*dotValues)[i] < pivot)i++;
		while ((*dotValues)[j] > pivot)j--;
		if (i <= j) {
			tempTri = (*triangles_)[i];
			(*triangles_)[i] = (*triangles_)[j];
			(*triangles_)[j] = tempTri;

			tempDot = (*dotValues)[i];
			(*dotValues)[i] = (*dotValues)[j];
			(*dotValues)[j] = tempDot;

			i++; j--;
		}
	} while (i <= j);

	if (lo < j) SortTriangles(vertices_, triangles_, dotValues, lo, j);
	if (i < hi) SortTriangles(vertices_, triangles_, dotValues, i, hi);
}


void Mesh::RearrangeFace(){
	//get angle between vector (0, 0, -1)
	glm::vec3 refVec = glm::vec3(0, 0, -1);
	
	vector<float> dotValueTri;
	for (int loop = 0; loop < triangles.size(); loop++) {
		GLuint vi = triangles[loop].idx0;
		dotValueTri.push_back(glm::dot(vertices[vi].Position, refVec));
	}

	SortTriangles(&vertices, &triangles, &dotValueTri, 0, triangles.size() - 1);
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
