#include "SceneRenderData.h"



SceneRenderData::SceneRenderData()
{
}


SceneRenderData::~SceneRenderData()
{
}

void SceneRenderData::AddRenderer(BaseRenderer * rdr_) {
	renderers.push_back(rdr_);
}

void SceneRenderData::AddLight(BaseLight * light_) {
	lights.push_back(light_);
}
