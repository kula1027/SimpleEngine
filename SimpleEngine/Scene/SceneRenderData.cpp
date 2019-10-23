#include "SceneRenderData.h"

#include <Render/RenderBundle.h>
#include <Debugger/SP_Debugger.h>

SceneRenderData::SceneRenderData(){
	
}


SceneRenderData::~SceneRenderData(){
}

void SceneRenderData::AddRenderer(MeshRenderer * rdr_) {	
	if (dynamic_cast<Renderer_Forward*>(rdr_)) {
		renderQueue_Forward.push_back(rdr_);
	} else if (dynamic_cast<Renderer_Deferred*>(rdr_)) {
		renderQueue_Deferred.push_back(rdr_);
	} else {
		DebugError("Invalid Renderer Type");
	}
}
