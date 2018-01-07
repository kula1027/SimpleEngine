#include "SimpleMath.h"

const float SimpleMath::PI = 3.1415927f;

SimpleMath::SimpleMath() {
}


SimpleMath::~SimpleMath() {
}



/// <image url = "$(SolutionDir)/CommentImages/oangle.png"/>
//위 이미지의 반대 방향각임
float SimpleMath::OrientedAngle(vec2 v) {
	float dotVal = dot(vec2(0, 1), v);
	float angle = acosf(dotVal);
	if (v.x > 0){
		angle = SimpleMath::PI * 2 - angle;
	} 
	return angle;
}

float SimpleMath::DegreeToRadian(float value) {
	return value / 180.0f * PI;
}

float SimpleMath::RadianToDegree(float value) {
	return value / PI * 180.0f;
}

vec3 SimpleMath::DegreeToRadianV3(vec3 value) {
	return vec3(
		DegreeToRadian(value.x),
		DegreeToRadian(value.y),
		DegreeToRadian(value.z)
	);
}

vec3 SimpleMath::RadianToDegreeV3(vec3 value) {
	return vec3(
		RadianToDegree(value.x),
		RadianToDegree(value.y),
		RadianToDegree(value.z)
	);
}


