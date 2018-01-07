#include "MeshModifier.h"

#include "SphereRenderer.h"
#include "SimpleMath.h"

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

Mesh** MeshModifier::DivideByAngle(Mesh * mesh_, int vertDivideCount, int horiDivideCount, int** idxPosition_){
	glm::vec3 up = glm::vec3(0, 1, 0);
	vector<Triangle>* dividedTriangles = new vector<Triangle>[vertDivideCount];

	//Vertical Division
	for (int loop = 0; loop < mesh_->triangles.size(); loop++) {//up과의 각에 따라 divide	
		glm::vec3 faceNormal = CalcFaceNormal(mesh_->triangles[loop], &mesh_->vertices);
		glm::vec3 faceCenter = CalcFaceCenter(mesh_->triangles[loop], &mesh_->vertices);

		glm::vec3 vColor;

		float angleBtw = acosf(glm::dot(faceNormal, up)) / SimpleMath::PI;//0 <= angleBtw <= 1
		angleBtw = angleBtw * vertDivideCount;// 0 <= angleBtw <= divisionCount_		
		if (angleBtw >= vertDivideCount)angleBtw--;//0 <= angleBtw < divisionCount_
		dividedTriangles[(int)angleBtw].push_back(mesh_->triangles[loop]);
		vColor = glm::vec3(1, 1, 1);
		
		for (int loop2 = 0; loop2 < 3; loop2++) {
			mesh_->vertices[mesh_->triangles[loop].idx[loop2]].color = vColor;
		}
	}

	/// <image url = "$(SolutionDir)/CommentImages/horiDivision.png"/>
	glm::vec3 refVec = glm::vec3(0, 0, 1);
			
	
	for (int loop = 0; loop < vertDivideCount; loop++) {		
		vector<float> angleHoris;		
		
		for (int loop2 = 0; loop2 < dividedTriangles[loop].size(); loop2++) {
			glm::vec3 faceNormal = CalcFaceNormal(dividedTriangles[loop][loop2], &mesh_->vertices);
			glm::vec2 faceNormal2d = glm::normalize(glm::vec2(faceNormal.x, faceNormal.z));
			
			float angle = SimpleMath::OrientedAngle(faceNormal2d);// 0 <= angle <= 2pi

			if (isnan(angle)) {
				angle = 0.0f;
			}
			angleHoris.push_back(angle);
			
		}

		SortTriangles(&dividedTriangles[loop], &angleHoris, 0, dividedTriangles[loop].size() - 1);		
				
		memset(idxPosition_[loop], -1, sizeof(int) * horiDivideCount);		
		for (int loop2 = 0; loop2 < angleHoris.size(); loop2++) {\
			
			float dv = angleHoris[loop2] / (SimpleMath::PI * 2) * horiDivideCount;//0 <= dv <= horiDivision			
			if (isnan(dv)){
				cout << endl;
			}				
			//	<< angleHoris[loop2] << endl;
			(int)dv >= horiDivideCount ? dv-- : NULL;
			if (idxPosition_[loop][(int)dv] < loop2) {
				idxPosition_[loop][(int)dv] = loop2;
			}
		}

		for (int loop2 = 0; loop2 < horiDivideCount; loop2++) {//빈 세그먼트에 대한 처리
			if (idxPosition_[loop][loop2] == -1) {
				if (loop2 == 0) {				
					idxPosition_[loop][loop2] = 0;
				} else {
					idxPosition_[loop][loop2] = idxPosition_[loop][loop2 - 1];
				}				
			}
		}
		 
		cout << "MeshVertical " << loop << " / FaceCount: " << dividedTriangles[loop].size() << endl;
		for (int loop2 = 0; loop2 < horiDivideCount; loop2++) {
			//cout << "\t" << loop2 << " : " << idxPosition_[loop][loop2] << endl;
		}		
	}

	Mesh** dividedMesh = new Mesh*[vertDivideCount];
	for (int loop = 0; loop < vertDivideCount; loop++) {
		dividedMesh[loop] = new Mesh();
	}

	//divided triangle을 토대로 divided mesh를 구성한다.
	for (int loop = 0; loop < vertDivideCount; loop++) {//각 mesh마다
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
					Vertex v = mesh_->vertices[indice[loop3]];

					dividedMesh[loop]->vertices.push_back(v);
					vertexIndirector[indice[loop3]] = dividedMesh[loop]->vertices.size() - 1;
					tri.idx[loop3] = idx++;
				} else {
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
