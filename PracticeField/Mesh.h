#pragma once
// Std. Includes
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <vector>
using namespace std;
// GL Includes
#include <GL/glew.h> // Contains all the necessery OpenGL includes
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

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

struct Texture {
	GLuint id;
	string type;
	string path;		
};

class Mesh {
public:
	/*  Mesh Data  */
	vector<Vertex> vertices;
	vector<Triangle> triangles;
	vector<Texture> textures;
	/*  Functions  */
	Mesh(vector<Vertex> vertices, vector<Triangle> triangles, vector<Texture> textures);
	GLuint VAO;

private:
	/*  Render data  */
	GLuint VBO, EBO;
	/*  Functions    */
	void RearrangeFace();
	void SetupMesh();
};


