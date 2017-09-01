#pragma once

#include "Mesh.h"

struct ArrangeMapper {
	float angle;
	GLuint idx;
};

const int arMapSize = 360;

class ArrangedMesh : public Mesh
{
private:
	void SortTriangles(vector<Triangle>* triangles_, vector<float>* dotValues, int lo, int hi);
	void RearrangeFace(glm::vec3 refVec_);

	unsigned int* EBOs;

protected:
	void SetupMesh();

public:
	ArrangedMesh(Mesh mesh_);
	~ArrangedMesh();

	ArrangeMapper* arMap;
};

