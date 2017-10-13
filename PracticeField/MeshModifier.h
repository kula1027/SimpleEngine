#pragma once
#include "Mesh.h"

class MeshModifier
{
public:
	MeshModifier();
	~MeshModifier();

	static void SortTriangles(vector<Triangle>* triangles_, vector<float>* dotValues, int lo, int hi);
	static Mesh** DivideVertical(Mesh* mesh_, glm::vec3 centerPos, int count_);
};

