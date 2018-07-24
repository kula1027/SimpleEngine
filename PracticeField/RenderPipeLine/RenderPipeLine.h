#pragma once

class RenderData;

class RenderPipeLine
{
public:
	RenderPipeLine();
	~RenderPipeLine();

	virtual void Render(RenderData* renderData) = 0;
};

