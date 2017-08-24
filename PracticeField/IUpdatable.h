#pragma once

#include "IComponent.h"

class IUpdatable : IComponent{
public:
	virtual void Update() = 0;
};

