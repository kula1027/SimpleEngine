#include "Mesh.h"

#include <Shaders/BaseShader.h>
#include "../Render/Texture.h"

#include <fstream>
#include <sstream>

Mesh::Mesh() {			
}

Mesh::~Mesh(){
}

void Mesh::Setup(){
	if (vertices.size() <= 0)return;

	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);

	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices[0], GL_STATIC_DRAW);

	glEnableVertexAttribArray(AttrLoc_Position);
	glVertexAttribPointer(AttrLoc_Position, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)0);//이후 shader에서 참조할때 정의된 attribute에 따라 vbo에서 가져옴
	//0번째 attribute를 정의
																			
	glEnableVertexAttribArray(AttrLoc_Normal);
	glVertexAttribPointer(AttrLoc_Normal, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, normal));

	glEnableVertexAttribArray(AttrLoc_TexCoord);
	glVertexAttribPointer(AttrLoc_TexCoord, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, texCoords));

	glEnableVertexAttribArray(AttrLoc_Color);
	glVertexAttribPointer(AttrLoc_Color, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, color));

	glGenBuffers(1, &EBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, triangles.size() * sizeof(Triangle),
		&triangles[0], GL_STATIC_DRAW);
}

void Mesh::UpdateBuffer() {
	if (vertices.size() <= 0)return;
	
	glBindVertexArray(VAO);	
	glBindBuffer(GL_ARRAY_BUFFER, VBO);

	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices[0], GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, triangles.size() * sizeof(Triangle),
		&triangles[0], GL_STATIC_DRAW);
}


int Mesh::GetIdxCount() {
	return triangles.size() * 3;
}

Mesh::Mesh(vector<Vertex> vertices_, vector<Triangle> triangles_, RenderMaterial * renderMaterials_) {
	this->vertices = vertices_;
	this->triangles = triangles_;
	this->renderMaterial = renderMaterials_;

	Setup();
}
