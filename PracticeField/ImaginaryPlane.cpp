#include "ImaginaryPlane.h"



ImaginaryPlane::ImaginaryPlane(vec3 normalVector_, vec3 point_) {
	normalVector = glm::normalize(normalVector_);
	point = point_;
	d = -(normalVector.x * point.x + normalVector.y * point.y + normalVector.z * point.z);
}


ImaginaryPlane::~ImaginaryPlane() {
}
