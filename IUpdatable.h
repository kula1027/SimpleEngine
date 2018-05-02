#pragma once

#include "IComponent.h"

class Transform;
class GameObject;

class IUpdatable : public IComponent{
public:	
	virtual void OnUpdate() {}
	virtual void OnStart() {}

	Transform* transform;
	GameObject* gameObject;
};

