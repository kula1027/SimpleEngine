#pragma once
// Std. Includes
#include <string>
#include <iostream>
#include <vector>
using namespace std;
// GL Includes
#include <GL/glew.h> // Contains all the necessery OpenGL includes
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

class Texture;

struct Vertex {
	// Position
	glm::vec3 Position;
	// Normal
	glm::vec3 Normal;
	// TexCoords
	glm::vec2 TexCoords;

	float dotValue;
};

struct Triangle {
	GLuint idx0;
	GLuint idx1;
	GLuint idx2;
};

class Mesh {
public:
	vector<Vertex> vertices;
	vector<Triangle> triangles;
	vector<Texture*> textures;
	GLuint VAO, VBO, EBO;
	GLuint instanceVBO;

	Mesh();
	Mesh(vector<Vertex> vertices, vector<Triangle> triangles, vector<Texture*> textures);
};


