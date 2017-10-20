#pragma once
#include "Mesh.h"

class MeshModifier
{
public:
	MeshModifier();
	~MeshModifier();

	static void SortTriangles(vector<Triangle>* triangles_, vector<float>* dotValues, int lo, int hi);
	static Mesh** DivideVertical(Mesh* mesh_, float min_y_, float height_, int count_);

	static glm::vec3 CalcFaceNormal(Triangle face_, vector<Vertex>* vertices_);
};

