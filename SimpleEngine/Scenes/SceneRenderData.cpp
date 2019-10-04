#include "SceneRenderData.h"

#include <Render/MeshRenderer.h>
#include <Lights/LightManager.h>

SceneRenderData::SceneRenderData(){
	
}


SceneRenderData::~SceneRenderData(){
}

void SceneRenderData::AddRenderer(MeshRenderer * rdr_) {	
	if (rdr_->GetRenderModeForward()) {
		renderQueue_Forward.push_back(rdr_);
	} else {
		renderQueue_Deferred.push_back(rdr_);		
	}
}
