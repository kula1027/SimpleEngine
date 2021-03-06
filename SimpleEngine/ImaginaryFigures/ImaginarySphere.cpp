#include "ImaginarySphere.h"

#include "../Mesh/Mesh.h"
#include "../Bases/Camera.h"
#include "../FilePooler.h"

ImaginarySphere::ImaginarySphere() {
}

ImaginarySphere::ImaginarySphere(vec3 center_, float radius_) {
	center = center_;
	radius = radius_;
}


ImaginarySphere::~ImaginarySphere() {
}

ImaginarySphere * ImaginarySphere::GetBoundingSphere(Mesh * mesh, glm::vec3 center_) {
	float largestDist = -1;	
	for (int loop = 0; loop < mesh->vertices.size(); loop++) {
		float dist = glm::distance(center_, mesh->vertices[loop].position);

		if (dist > largestDist) {
			largestDist = dist;
		}		
	}

	return new ImaginarySphere(center_, largestDist);
}
