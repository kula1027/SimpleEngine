#include "SceneRenderData.h"

#include <Render/Renderer/RendererBundle.h>
#include <Debugger/SP_Debugger.h>

SceneRenderData::SceneRenderData() {

}


SceneRenderData::~SceneRenderData() {
}

void SceneRenderData::AddRenderer(MeshRenderer * rdr_) {
	switch (rdr_->GetRenderPathType()) {
	case RenderType_Deferred:
		renderQueue_Deferred.push_back(rdr_);
		break;

	case RenderType_Forward:
		renderQueue_Forward.push_back(rdr_);
		break;

	default:
		DebugError("Invalid Renderer Type: " + rdr_->GetRenderPathType());
		break;
	}
}
