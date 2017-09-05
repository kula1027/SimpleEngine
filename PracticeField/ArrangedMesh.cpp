#include "ArrangedMesh.h"



ArrangedMesh::ArrangedMesh(Mesh mesh_){
	this->vertices = mesh_.vertices;
	this->triangles = mesh_.triangles;
	this->textures = mesh_.textures;

	this->VAO = mesh_.VAO;
	this->VBO = mesh_.VBO;
	this->EBO = mesh_.EBO;

	EBOs = new unsigned int[samplingDirCount];
	arrangedTriangles = new vector<Triangle>[samplingDirCount];
	double angle = 0;
	for (int loop = 0; loop < samplingDirCount; loop++) {
		cout << "Sampling Dir... " << loop << endl;
		angle = 3.141592f / samplingDirCount * loop;
		this->RearrangeFace(glm::vec3(0, sin(angle), cos(angle)), loop);
		
	}

	this->SetupMesh();
}


ArrangedMesh::~ArrangedMesh(){
	delete(arMap);
	delete(EBOs);
	delete(arrangedTriangles);
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


void ArrangedMesh::RearrangeFace(glm::vec3 refVec_, int idx_) {
	//get angle between refVec and
	glm::vec3 refVec = refVec_;

	double angle = 3.141592f / samplingDirCount * idx_ + 3.141592f / 180.0f * 90.0f;//TODO
	glm::vec3 planeNormal = glm::vec3(0, sin(angle), cos(angle));

	vector<Triangle> tri0;// x < 0
	vector<glm::vec3> tri0_normal;// x < 0
	vector<Triangle> tri1;// x >= 0
	vector<glm::vec3> tri1_normal;// x >= 0

	for (int loop = 0; loop < triangles.size(); loop++) {
		GLuint vi0 = triangles[loop].idx0;
		GLuint vi1 = triangles[loop].idx1;
		GLuint vi2 = triangles[loop].idx2;

		glm::vec3 dir0 = vertices[vi1].Position - vertices[vi0].Position;
		glm::vec3 dir1 = vertices[vi2].Position - vertices[vi1].Position;
		glm::vec3 tempNorm = glm::cross(dir0, dir1);

		if (tempNorm.x < 0) {
			tri0.push_back(triangles[loop]);
			tri0_normal.push_back(glm::normalize(tempNorm));
		}
		else {
			tri1.push_back(triangles[loop]);
			tri1_normal.push_back(glm::normalize(tempNorm));
		}
	}

	//Set & Sort by dot values
	vector<float> dotValueTri0;
	for (int loop = 0; loop < tri0.size(); loop++) {		
		glm::vec3 faceNorm = tri0_normal[loop];
		faceNorm = faceNorm - glm::dot(faceNorm, planeNormal) * planeNormal;
		faceNorm = glm::normalize(faceNorm);		
		dotValueTri0.push_back(glm::dot(faceNorm, refVec));		
	}
	SortTriangles(&tri0, &dotValueTri0, 0, tri0.size() - 1);

	vector<float> dotValueTri1;
	for (int loop = 0; loop < tri1.size(); loop++) {		
		glm::vec3 faceNorm = tri1_normal[loop];
		faceNorm = faceNorm - glm::dot(faceNorm, planeNormal) * planeNormal;
		faceNorm = glm::normalize(faceNorm);
		dotValueTri1.push_back(glm::dot(faceNorm, refVec));
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
		float val = dotValueTri0[loop] + 1; // 0 ~ 2
		if (val > arMap[arIdx].angle) {
			arMap[arIdx].idx = loop;			
			arIdx++;
		}
	}
	for (int loop = 0; loop < sizeValTri1; loop++) {
		float val = dotValueTri1[loop] + 3; // 2 ~ 4
		if (val > arMap[arIdx].angle) {
			arMap[arIdx].idx = loop + sizeValTri0;
			arIdx++;
		}
	}
	for (int loop = 0; loop < sizeValTri0; loop++) {//추가하는 중복 face의 수를 최소화 하는 refVec을 찾으면 더 효율적
		float val = dotValueTri0[loop] + 5; // 4 ~ 6
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

	arrangedTriangles[idx_] = rearrangedTriangles;
}

void ArrangedMesh::SetupMesh(){
	glBindVertexArray(this->VAO);

	glGenBuffers(samplingDirCount, EBOs);
	for (int loop = 0; loop < samplingDirCount; loop++) {
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBOs[loop]);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, this->arrangedTriangles[loop].size() * sizeof(Triangle),
			&this->arrangedTriangles[loop][0], GL_STATIC_DRAW);
	}
	
	/*glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, this->triangles.size() * sizeof(Triangle),
		&this->triangles[0], GL_STATIC_DRAW);*/

	glBindVertexArray(0);
}
