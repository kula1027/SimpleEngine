#pragma once

class EngineObject;
class Transform;

class BaseComponent {
private:
	EngineObject* engineObject;
	Transform* transform;

public:
	virtual void OnAttachedToObject(EngineObject* obj_);

	EngineObject* GetEngineObject();
	Transform* GetTransform();

	virtual ~BaseComponent();
};