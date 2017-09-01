#include "ArrangedMesh.h"



ArrangedMesh::ArrangedMesh(Mesh mesh_){
	this->vertices = mesh_.vertices;
	this->triangles = mesh_.triangles;
	this->textures = mesh_.textures;

	this->VAO = mesh_.VAO;
	this->VBO = mesh_.VBO;
	this->EBO = mesh_.EBO;

	EBOs = new unsigned int[4];

	this->RearrangeFace(glm::vec3(0, 0, 1));
	this->SetupMesh();
}


ArrangedMesh::~ArrangedMesh(){
	delete(arMap);
}

void ArrangedMesh::SortTriangles(vector<Triangle>* triangles_, vector<float>* dotValues, int lo, int hi) {
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


void ArrangedMesh::RearrangeFace(glm::vec3 refVec_) {
	//get angle between refVec and
	glm::vec3 refVec = glm::vec3(0, 0, 1);
	glm::vec3 planeNormal = glm::vec3(0, 1, 0);

	vector<Triangle> tri0;// x < 0
	vector<Triangle> tri1;// x >= 0

	for (int loop = 0; loop < triangles.size(); loop++) {
		GLuint vi = triangles[loop].idx0;
		if (vertices[vi].Position.x < 0) {
			tri0.push_back(triangles[loop]);
		}
		else {
			tri1.push_back(triangles[loop]);
		}
	}

	//Set & Sort by dot values
	vector<float> dotValueTri0;
	for (int loop = 0; loop < tri0.size(); loop++) {
		GLuint vi = tri0[loop].idx0;
		glm::vec3 tempV = vertices[vi].Position;
		tempV = tempV - glm::dot(tempV, planeNormal) * planeNormal;
		tempV = glm::normalize(tempV);
		glm::vec3 v = glm::normalize(glm::vec3(vertices[vi].Position.x, 0, vertices[vi].Position.z));
		dotValueTri0.push_back(glm::dot(tempV, refVec));
	}
	SortTriangles(&tri0, &dotValueTri0, 0, tri0.size() - 1);

	vector<float> dotValueTri1;
	for (int loop = 0; loop < tri1.size(); loop++) {
		GLuint vi = tri1[loop].idx0;
		glm::vec3 tempV = vertices[vi].Position;
		tempV = tempV - glm::dot(tempV, planeNormal) * planeNormal;
		tempV = glm::normalize(tempV);
		glm::vec3 v = glm::normalize(glm::vec3(vertices[vi].Position.x, 0, vertices[vi].Position.z));
		dotValueTri1.push_back(glm::dot(tempV, refVec));
	}
	SortTriangles(&tri1, &dotValueTri1, 0, tri1.size() - 1);

	//Set arMap
	arMap = new ArrangeMapper[arMapSize];
	for (int loop = 0; loop < arMapSize; loop++) {
		arMap[loop].angle = (6 * loop) / (float)arMapSize;
	}
	
	int arIdx = 0;
	int sizeValTri0 = dotValueTri0.size();
	int sizeValTri1 = dotValueTri1.size();
	for (int loop = 0; loop < sizeValTri0; loop++) {
		float val = dotValueTri0[loop] + 1;
		if (val > arMap[arIdx].angle) {
			arMap[arIdx].idx = loop;			
			arIdx++;
		}
	}
	for (int loop = 0; loop < sizeValTri1; loop++) {
		float val = dotValueTri1[loop] + 3;
		if (val > arMap[arIdx].angle) {
			arMap[arIdx].idx = loop + sizeValTri0;
			arIdx++;
		}
	}
	for (int loop = 0; loop < sizeValTri0; loop++) {//추가하는 중복 face의 수를 최소화 하는 refVec을 찾으면 더 효율적
		float val = dotValueTri0[loop] + 5;
		if (val > arMap[arIdx].angle) {
			arMap[arIdx].idx = loop + sizeValTri0 + sizeValTri1;
			arIdx++;			
			if (arIdx >= arMapSize)break;
		}
	}

	//ref기준 정렬해 넣는다
	vector<Triangle> rearrangedTriangles;
	for (int loop = tri0.size() - 1; loop >= 0; loop--) {
		rearrangedTriangles.push_back(tri0[loop]);
	}
	for (int loop = 0; loop < tri1.size(); loop++) {
		rearrangedTriangles.push_back(tri1[loop]);
	}
	for (int loop = tri0.size() - 1; loop >= 0; loop--) {
		rearrangedTriangles.push_back(tri0[loop]);
	}

	triangles = rearrangedTriangles;

}

void ArrangedMesh::SetupMesh(){
	glBindVertexArray(this->VAO);

	glGenBuffers(4, EBOs);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBOs[0]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, this->triangles.size() * sizeof(Triangle),
		&this->triangles[0], GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, this->triangles.size() * sizeof(Triangle),
		&this->triangles[0], GL_STATIC_DRAW);

	glBindVertexArray(0);
}
