#pragma once

#include "Mesh.h"

struct ArrangeMapper {
	float angle;
	GLuint idx;
};

struct SampleData {
	glm::vec3 refVec;
	glm::vec3 planeNormal;
};

const unsigned int arMapSize = 120;//¹è¿­ °¹¼ö´Â arMapSize + 1
const unsigned int samplingDirCount = 6;

class ArrangedMesh : public Mesh
{
private:
	void SortTriangles(vector<Triangle>* triangles_, vector<float>* dotValues, int lo, int hi);
	void RearrangeFace(int idx_);

	
	vector<Triangle>* arrangedTriangles;

protected:
	void SetupMesh();

public:
	ArrangedMesh(Mesh mesh_);
	~ArrangedMesh();

	unsigned int* EBOs;
	ArrangeMapper* arMap;
	SampleData* sampleData;
};

