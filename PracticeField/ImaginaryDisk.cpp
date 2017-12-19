#include "ImaginaryDisk.h"



ImaginaryDisk::ImaginaryDisk() {
}

ImaginaryDisk::ImaginaryDisk(vec3 upCenter_, float height_, float radius_) {
	up = glm::vec3(0, 1, 0);
	upCenter = upCenter_;
	height = height_;
	radius = radius_;
}


ImaginaryDisk::~ImaginaryDisk() {
}
