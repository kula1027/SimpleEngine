#include "RP_Deferred.h"

#include <gl/glew.h>

RP_Deferred::RP_Deferred() {
}


RP_Deferred::~RP_Deferred() {
}

void RP_Deferred::Initialize() {
	InitOffScreenDraw();
}

void RP_Deferred::Render(Camera * mainCamera, SceneRenderData * renderData) {
}
