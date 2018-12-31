#pragma once

class IComponent {
public:
	virtual void Initialize() {}
	virtual void Destroy() {}
};