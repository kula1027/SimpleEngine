#pragma once

class EngineObject;
class Transform;

class BaseComponent {
private:
	static unsigned int freeComponentId;

	EngineObject* engineObject;
	Transform* transform;
	unsigned int componentId;
	unsigned int GetFreeComponentId();

	bool isEnabled;	

protected:
	

public:
	BaseComponent();

	virtual void OnAttachedToObject(EngineObject* obj_);
	virtual void OnTransformChanged() {}

	EngineObject* GetEngineObject();
	Transform* GetTransform();
	unsigned int GetComponentId();

	virtual ~BaseComponent();

	void SetEnabled(bool enabled_);
	bool GetEnabled();
};