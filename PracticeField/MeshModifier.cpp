#include "MeshModifier.h"



MeshModifier::MeshModifier()
{
}


MeshModifier::~MeshModifier()
{
}

void MeshModifier::SortTriangles(vector<Triangle>* triangles_, vector<float>* dotValues, int lo, int hi){
	int i = lo, j = hi;
	Triangle tempTri;
	float tempDot;
	float pivot = (*dotValues)[(lo + hi) / 2];

	do {
		while ((*dotValues)[i] < pivot)i++;
		while ((*dotValues)[j] > pivot)j--;
		if (i <= j) {
			tempTri = (*triangles_)[i];
			(*triangles_)[i] = (*triangles_)[j];
			(*triangles_)[j] = tempTri;

			tempDot = (*dotValues)[i];
			(*dotValues)[i] = (*dotValues)[j];
			(*dotValues)[j] = tempDot;

			i++; j--;
		}
	} while (i <= j);

	if (lo < j) SortTriangles(triangles_, dotValues, lo, j);
	if (i < hi) SortTriangles(triangles_, dotValues, i, hi);
}

Mesh** MeshModifier::DivideVertical(Mesh * mesh_, float min_y_, float height_, int divisionCount_){
	glm::vec3 up = glm::vec3(0, 1, 0);
	vector<Triangle>* dividedTriangles = new vector<Triangle>[divisionCount_];
	float segmentHeight = height_ / divisionCount_;

	for (int loop = 0; loop < mesh_->triangles.size(); loop++) {
		float centerY = 0;
		glm::vec3 faceNormal = CalcFaceNormal(mesh_->triangles[loop], &mesh_->vertices);
		float dotVal = glm::dot(faceNormal, up);
		dotVal = (dotVal + 1) / 2 * divisionCount_;// 0 <= dotVal < divisionCount_		
		if (dotVal >= divisionCount_)dotVal = divisionCount_ - 1;
		dividedTriangles[(int)dotVal].push_back(mesh_->triangles[loop]);
	}

	for (int loop = 0; loop < divisionCount_; loop++) {
		vector<float> dotValues;

		for (int loop2 = 0; loop2 < dividedTriangles[loop].size(); loop2++) {
	//		float dotValue = glm::dot(glm::vec3(0, 0, 1), ;
	//		dotValues.push_back(dotValue);
		}
	}


	Mesh** dividedMesh = new Mesh*[divisionCount_];
	for (int loop = 0; loop < divisionCount_; loop++) {
		dividedMesh[loop] = new Mesh();
	}

	//divided triangle을 토대로 divided mesh를 구성한다.
	for (int loop = 0; loop < divisionCount_; loop++) {//각 mesh마다
		int *vertexIndirector = new int[mesh_->vertices.size()];
		memset(vertexIndirector, -1, sizeof(int) * mesh_->vertices.size());

		int idx = 0;
		for (int loop2 = 0; loop2 < dividedTriangles[loop].size(); loop2++) {
			int indice[3] = {
				dividedTriangles[loop][loop2].idx[0],
				dividedTriangles[loop][loop2].idx[1],
				dividedTriangles[loop][loop2].idx[2]
			};

			Triangle tri;
			for (int loop3 = 0; loop3 < 3; loop3++) {
				if (vertexIndirector[indice[loop3]] == -1) {//처음보는 vertex
					Vertex ttt = mesh_->vertices[indice[loop3]];
					//ttt.position.y += (loop - 5) * 0.5f;
					dividedMesh[loop]->vertices.push_back(ttt);
					vertexIndirector[indice[loop3]] = dividedMesh[loop]->vertices.size() - 1;
					tri.idx[loop3] = idx++;
				}
				else {
					tri.idx[loop3] = vertexIndirector[indice[loop3]];
				}
			}
			
			dividedMesh[loop]->triangles.push_back(tri);
		}

		delete vertexIndirector;
	}

	delete[] dividedTriangles;
	

	return dividedMesh;
}

glm::vec3 MeshModifier::CalcFaceNormal(Triangle face_, vector<Vertex>* vertices_){
	GLuint vi0 = face_.idx[0];
	GLuint vi1 = face_.idx[1];
	GLuint vi2 = face_.idx[2];

	glm::vec3 dir0 = vertices_->at(vi1).position - vertices_->at(vi0).position;
	glm::vec3 dir1 = vertices_->at(vi2).position - vertices_->at(vi1).position;
	
	return glm::normalize(glm::cross(dir0, dir1));
}
