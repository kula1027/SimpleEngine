#include "EngineResource.h"



EngineResource::EngineResource(){
	usageCount = 0;
}


EngineResource::~EngineResource()
{
}

void EngineResource::Release(){
	usageCount--;
	if (usageCount <= 0) {
		OnEndUse();
	}
}

void EngineResource::MarkUse(){
	usageCount++;
}
