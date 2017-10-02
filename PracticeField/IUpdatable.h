#pragma once

#include "IComponent.h"

class IUpdatable : IComponent{
public:
	virtual void Update() = 0;
	virtual void OnStart() = 0;
};

