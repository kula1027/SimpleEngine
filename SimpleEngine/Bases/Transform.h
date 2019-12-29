#pragma once

#include <glm\glm.hpp>
#include <glm\gtc\quaternion.hpp>
#include <glm\gtx\quaternion.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <vector>
#include <Bases/BaseComponent.h>

class EngineObject;

using namespace glm;
class Transform : public BaseComponent
{
private:
	EngineObject* engineObject;
	Transform* parent = NULL;
	std::vector<Transform*> children;

	mat4 modelMatrix;

	vec3 forward;
	vec3 right;
	vec3 up;

	vec3 position;
	vec3 scale;

	quat rotation;
	vec3 eulerAngles;

	void OnChange();

	bool nuModelMatrix;
	void CalcModelMatrix();

public:
	Transform(EngineObject* engineObject_);
	~Transform();

	EngineObject* GetEngineObject();

	Transform* GetParent();
	void SetParent(Transform* parent_);

	mat4 GetModelMatrix();

	vec3 GetPosition();
	void SetPosition(vec3 position_);
	void AddPosition(vec3 addVec3);	

	vec3 GetScale();
	void SetScale(vec3 scale_);

#pragma region Children
	int GetChildCount();
	Transform* GetChildAt(int idx_);
	void AddChild(Transform* transform_);
	void RemoveChildAt(int idx_);

#pragma endregion

#pragma region Rotation	
	vec3 GetEulerAngles();
	void SetEulerAngles(vec3 eulerAngles_);

	vec3 GetForward();
	void SetForward(vec3 eulerAngles_);

	vec3 GetRight();
	void SetRight(vec3 eulerAngles_);

	vec3 GetUp();
	void SetUp(vec3 eulerAngles_);

	void Rotate(vec3 eulerAngles_);
#pragma endregion			
};

