#pragma once

#include "BaseComponent.h"

class Transform;
class EngineObject;

class BaseScript : public BaseComponent{
public:	
	virtual void OnUpdate() {}
	virtual void OnStart() {}

	Transform* transform;
	EngineObject* gameObject;
};

