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

Mesh** MeshModifier::DivideVertical(Mesh * mesh_, glm::vec3 centerPos, int count_){
	glm::vec3 up = glm::vec3(0, 1, 0);
	vector<Triangle>* dividedTriangles = new vector<Triangle>[count_];

	for (int loop = 0; loop < mesh_->triangles.size(); loop++) {
		int idx = mesh_->triangles[loop].idx0;
		glm::vec3 vPos = mesh_->vertices[idx].Position;
		glm::vec3 v = centerPos - vPos;
		v = glm::normalize(v);
		float dotValue = (glm::dot(v, up) + 1) / 2 * count_;
		if (dotValue >= count_)dotValue = count_ - 1;
		dividedTriangles[(int)dotValue].push_back(mesh_->triangles[loop]);
	}

	Mesh** dividedMesh = new Mesh*[count_];
	for (int loop = 0; loop < count_; loop++) {
		dividedMesh[loop] = new Mesh();
	}
	/*for (int loop = 0; loop < mesh_->triangles.size(); loop++) {
		GLuint vi0 = mesh_->triangles[loop].idx0;
		GLuint vi1 = mesh_->triangles[loop].idx1;
		GLuint vi2 = mesh_->triangles[loop].idx2;

		glm::vec3 dir0 = mesh_->vertices[vi1].Position - mesh_->vertices[vi0].Position;
		glm::vec3 dir1 = mesh_->vertices[vi2].Position - mesh_->vertices[vi1].Position;
		glm::vec3 tempNorm = glm::cross(dir0, dir1);

		if (tempNorm.x < 0) {
			tri0.push_back(mesh_->triangles[loop]);
			
		}
		else {
			tri1.push_back(mesh_->triangles[loop]);
			
		}
	}*/

	return dividedMesh;
}
