#pragma once
#include "Mesh.h"

class MeshModifier
{
public:
	MeshModifier();
	~MeshModifier();

	static void SortTriangles(vector<Triangle>* triangles_, vector<FaceData>* faceNormal, vector<float>* dotValues, int lo, int hi);
	static Mesh** RearrangeMeshByNormal_Split(Mesh* mesh_, int vertDivideCount, int horiDivideCount, int** idxPosition_);
	static int** RearrangeMeshByNormal(Mesh* mesh_, int vertDivideCount, int horiDivideCount, int** idxPosition_);

	static glm::vec3 CalcFaceNormal(Triangle face_, vector<Vertex>* vertices_);
	static glm::vec3 CalcFaceCenter(Triangle face_, vector<Vertex>* vertices_);
};