#pragma once

#include <glm\glm.hpp>
#include <glm\gtc\quaternion.hpp>
#include <glm\gtx\quaternion.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <vector>

class EngineObject;

using namespace glm;
class Transform
{
private:
	vec3 forward;
	vec3 right;
	vec3 up;

	quat rotation;
	vec3 eulerAngles;

public:
	Transform();
	vec3 position;	
	vec3 scale;	


#pragma region Rotation	
	vec3 GetEulerAngles();
	void SetEulerAngles(vec3 degreeV3);

	vec3 GetForward();
	void SetForward(vec3 degreeV3);

	vec3 GetRight();
	void SetRight(vec3 degreeV3);

	vec3 GetUp();
	void SetUp(vec3 degreeV3);

	void Rotate(vec3 eulerAngles);
#pragma endregion
	EngineObject* engineObject;

	std::vector<Transform*> children;
	Transform* parent = NULL;

	void SetParent(Transform* parent_);
	mat4 GetMatrix4();
	~Transform();
};

