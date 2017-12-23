#include "Calculator.h"

const float Calculator::PI = 3.1415927f;

Calculator::Calculator() {
}


Calculator::~Calculator() {
}



/// <image url = "$(SolutionDir)/CommentImages/oangle.png"/>
//위 이미지의 반대 방향각임
float Calculator::OrientedAngle(vec2 v) {
	float dotVal = dot(vec2(0, 1), v);
	float angle = acosf(dotVal);
	if (v.x > 0){
		angle = Calculator::PI * 2 - angle;
	} 
	return angle;
}
