#pragma once
class EngineResource
{
private:
	int usageCount;
public:
	EngineResource();
	~EngineResource();

	void Release();
	void MarkUse();

	virtual void OnEndUse() = 0;

	bool destroyOnEndUse;
};

