#include "Mesh.h"

#include "Shader.h"
#include "Texture.h"

#include <fstream>
#include <sstream>

Mesh::Mesh() {
	isStatic = true;
	isSetup = false;
}

Mesh::~Mesh(){
}

void Mesh::Setup(){
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);

	if (isStatic) {
		glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices[0], GL_STATIC_DRAW);
	} else {
		glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices[0], GL_DYNAMIC_DRAW);
	}

	glEnableVertexAttribArray(AttrLoc_Position);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)0);//현재 bind된 buffer의 내용을 기술된 attribute대로 vao에 박아넣음
																			
	glEnableVertexAttribArray(AttrLoc_Normal);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, normal));

	glEnableVertexAttribArray(AttrLoc_TexCoord);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, texCoords));

	glGenBuffers(1, &EBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	if (isStatic) {
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, triangles.size() * sizeof(Triangle),
			&triangles[0], GL_STATIC_DRAW);
	}
	else {
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, triangles.size() * sizeof(Triangle),
			&triangles[0], GL_DYNAMIC_DRAW);
	}

	glBindVertexArray(0);

	isSetup = true;
}

void Mesh::ResetupEbo(){
	glBindVertexArray(this->VAO);

	glGenBuffers(1, &EBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, triangles.size() * sizeof(Triangle),
		&this->triangles[0], GL_STATIC_DRAW);

	/*glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, this->triangles.size() * sizeof(Triangle),
	&this->triangles[0], GL_STATIC_DRAW);*/

	glBindVertexArray(0);
}

void Mesh::Resetup()
{
}

Mesh::Mesh(vector<Vertex> vertices_, vector<Triangle> triangles_, vector<Texture*> textures_){
	this->vertices = vertices_;
	this->triangles = triangles_;
	this->textures = textures_;
	isStatic = true;
	isSetup = false;
}
