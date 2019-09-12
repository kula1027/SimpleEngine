#pragma once

class EngineObject;

class BaseComponent {
private:
	EngineObject* engineObject;

public:
	virtual void OnAttachedToObject(EngineObject* obj_);

	EngineObject* GetAttachedObject();

	virtual ~BaseComponent();
};