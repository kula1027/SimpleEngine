#include "ImaginaryCube.h"

#include "../Mesh.h"

ImaginaryCube::ImaginaryCube() {
}

ImaginaryCube::ImaginaryCube(vec3 center_, vec3 max_, vec3 min_) {
	center = center_;
	max = max_;
	min = min_;
}


ImaginaryCube::~ImaginaryCube() {
}

ImaginaryCube* ImaginaryCube::GetBoundingBox(Mesh * mesh) {
	glm::vec3 box_max = glm::vec3(-99999, -99999, -99999);
	glm::vec3 box_min = glm::vec3(99999, 99999, 99999);

	for (int loop = 0; loop < mesh->vertices.size(); loop++) {
		if (mesh->vertices[loop].position.x > box_max.x) {
			box_max.x = mesh->vertices[loop].position.x;
		}
		if (mesh->vertices[loop].position.y > box_max.y) {
			box_max.y = mesh->vertices[loop].position.y;
		}
		if (mesh->vertices[loop].position.z > box_max.z) {
			box_max.z = mesh->vertices[loop].position.z;
		}

		if (mesh->vertices[loop].position.x < box_min.x) {
			box_min.x = mesh->vertices[loop].position.x;
		}
		if (mesh->vertices[loop].position.y < box_min.y) {
			box_min.y = mesh->vertices[loop].position.y;
		}
		if (mesh->vertices[loop].position.z < box_min.z) {
			box_min.z = mesh->vertices[loop].position.z;
		}
	}

	glm::vec3 box_center = (box_max + box_min) / 2.0f;

	return new ImaginaryCube(box_center, box_max, box_min);
}
