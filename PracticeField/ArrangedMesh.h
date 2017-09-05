#pragma once

#include "Mesh.h"

struct ArrangeMapper {
	float angle;
	GLuint idx;
};

const unsigned int arMapSize = 360;
const unsigned int samplingDirCount = 2;

class ArrangedMesh : public Mesh
{
private:
	void SortTriangles(vector<Triangle>* triangles_, vector<float>* dotValues, int lo, int hi);
	void RearrangeFace(glm::vec3 refVec_, int idx_);

	
	vector<Triangle>* arrangedTriangles;

protected:
	void SetupMesh();

public:
	ArrangedMesh(Mesh mesh_);
	~ArrangedMesh();

	unsigned int* EBOs;
	ArrangeMapper* arMap;
};

