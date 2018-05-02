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


#define AttrLoc_Position 0
#define AttrLoc_Normal 1
#define AttrLoc_TexCoord 2
#define AttrLoc_Color 3

class Texture;

struct Vertex {
	glm::vec3 position;
	glm::vec3 normal;
	glm::vec2 texCoords;
	glm::vec3 color;
};

struct Triangle {
	GLuint idx[3];
};

struct FaceData {
	glm::vec3 normal;
	glm::vec3 position;
};

class Mesh {
public:
	vector<Vertex> vertices;
	vector<Triangle> triangles;	
	vector<Texture*> textures;

	vector<FaceData> faceData;

	GLuint VAO, VBO, EBO;
	GLuint instanceVBO;
	bool isStaticDraw;
	bool isSetup;

	Mesh();
	~Mesh();

	void Setup();
	void ResetupEbo();
	void Resetup();
	int GetIdxCount();

	Mesh(vector<Vertex> vertices, vector<Triangle> triangles, vector<Texture*> textures);
};


