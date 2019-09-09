#pragma once
#include <Mesh/Mesh.h>

class MeshModifier
{
public:
	MeshModifier();
	~MeshModifier();

	static void SortTriangles(vector<Triangle>* triangles_, vector<FaceData>* faceNormal, vector<float>* dotValues, int lo, int hi);
	static Mesh** RearrangeMeshByNormal_Split(Mesh* mesh_, int vertDivideCount, int horiDivideCount, int** idxPosition_);
	static int** RearrangeMeshByNormal(Mesh* mesh_, int vertDivideCount, int horiDivideCount, int** idxPosition_);
	static int** RearrangeMeshByNormal_CacheTest(Mesh* mesh_, int vertDivideCount, int horiDivideCount, int** idxPosition_);

	static glm::vec3 CalcFaceNormal(Triangle face_, vector<Vertex>* vertices_);
	static glm::vec3 CalcFaceCenter(Triangle face_, vector<Vertex>* vertices_);

	static void CombineMeshes(Mesh** meshes, Mesh* combinedMesh, int meshCount);

private:
	static void SplitVertical(Mesh* mesh_, vector<Triangle>* dividedTriangles, int splitCount, glm::vec3 up);
	static void RearrangeHorizontal(Mesh* mesh_, vector<Triangle>* dividedTriangles, int ** idxPosition_, vector<FaceData>* listFaceData, int vCount, int hCount);
};