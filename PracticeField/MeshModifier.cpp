#include "MeshModifier.h"

#include "SphereRenderer.h"


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

Mesh** MeshModifier::DivideByAngle(Mesh * mesh_, int divisionCount_, int** idxPosition_){
	glm::vec3 up = glm::vec3(0, 1, 0);
	vector<Triangle>* dividedTriangles = new vector<Triangle>[divisionCount_];

	for (int loop = 0; loop < mesh_->triangles.size(); loop++) {//up���� ���� ���� divide	
		glm::vec3 faceNormal = CalcFaceNormal(mesh_->triangles[loop], &mesh_->vertices);
		glm::vec3 faceCenter = CalcFaceCenter(mesh_->triangles[loop], &mesh_->vertices);

		glm::vec3 vColor;

		float dotVal = glm::dot(faceNormal, up);
		dotVal = (dotVal + 1) / 2 * divisionCount_;// 0 <= dotVal < divisionCount_		
		if (dotVal >= divisionCount_)dotVal = divisionCount_ - 1;
		dividedTriangles[(int)dotVal].push_back(mesh_->triangles[loop]);
		vColor = glm::vec3(1, 1, 1);
		
		for (int loop2 = 0; loop2 < 3; loop2++) {
			mesh_->vertices[mesh_->triangles[loop].idx[loop2]].color = vColor;
		}
	}

	/// <image url = "$(SolutionDir)/CommentImages/horiDivision.png"/>
	glm::vec3 refVec = glm::vec3(0, 0, 1);
			
	for (int loop = 0; loop < divisionCount_; loop++) {
		vector<float> dotValues;		
		
		for (int loop2 = 0; loop2 < dividedTriangles[loop].size(); loop2++) {
			glm::vec3 faceNormal = CalcFaceNormal(dividedTriangles[loop][loop2], &mesh_->vertices);
			faceNormal.y = 0;			
			float dotVal = glm::dot(faceNormal, refVec);
			if (faceNormal.x < 0) {//0 ~ 4�� ���� ��ȯ, �ð���� 
				dotVal = -dotVal + 1;				
			}else {
				dotVal += 3;
			}

			dotValues.push_back(dotVal);
		}

		SortTriangles(&dividedTriangles[loop], &dotValues, 0, dividedTriangles[loop].size() - 1);		
		
		
		memset(idxPosition_[loop], -1, sizeof(int) * SphereRenderer::horiDivision);
		int currentIdx = 0;
		for (int loop2 = 0; loop2 < dotValues.size(); loop2++) {
			float dv = dotValues[loop2] * SphereRenderer::horiDivision / 4;//0 <= dv <= horiDivision
			(int)dv >= SphereRenderer::horiDivision ? dv-- : 0;
			if (idxPosition_[loop][(int)dv] < loop2) {
				idxPosition_[loop][(int)dv] = loop2;
			}
		}
		 
		cout << "DD: " << loop << endl;
		for (int loop2 = 0; loop2 < 8; loop2++) {
			cout << idxPosition_[loop][loop2] << endl;
		}		
	}

	Mesh** dividedMesh = new Mesh*[divisionCount_];
	for (int loop = 0; loop < divisionCount_; loop++) {
		dividedMesh[loop] = new Mesh();
	}

	//divided triangle�� ���� divided mesh�� �����Ѵ�.
	for (int loop = 0; loop < divisionCount_; loop++) {//�� mesh����
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
				if (vertexIndirector[indice[loop3]] == -1) {//ó������ vertex
					Vertex v = mesh_->vertices[indice[loop3]];										

					dividedMesh[loop]->vertices.push_back(v);
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

glm::vec3 MeshModifier::CalcFaceCenter(Triangle face_, vector<Vertex>* vertices_){
	glm::vec3 centerPos = glm::vec3(0);

	for (int loop = 0; loop < 3; loop++) {
		centerPos += vertices_->at(face_.idx[loop]).position;
	}

	return centerPos / 3.0f;
}
