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
class RenderMaterial;

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
private:
	void Setup();



public:
	vector<Vertex> vertices;
	vector<Triangle> triangles;	
	RenderMaterial* renderMaterial;
	vector<Texture*> textures;

	vector<FaceData> faceData;

	GLuint VAO, VBO, EBO;
	GLuint instanceVBO;

	Mesh();
	~Mesh();	
	Mesh(vector<Vertex> vertices, vector<Triangle> triangles, RenderMaterial* renderMaterials);

	void UpdateBuffer();
	int GetIdxCount();
};


