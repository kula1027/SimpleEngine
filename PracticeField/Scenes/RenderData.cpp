#include "RenderData.h"



RenderData::RenderData()
{
}


RenderData::~RenderData()
{
}

void RenderData::AddRenderer(Renderer * rdr_) {
	renderers.push_back(rdr_);
}

void RenderData::AddLight(BaseLight * light_) {
	lights.push_back(light_);
}
