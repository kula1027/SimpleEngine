#include "ShaderDeferredLight.h"
#include <string.h>
#include <vector>

#include <Lights/LightsBundle.h>

ShaderDeferredLight::ShaderDeferredLight() {
	filePathVertex = "Deferred/deferred_light.vert";
	filePathFragment = "Deferred/deferred_light.frag";

	LoadProgram(filePathVertex, "", filePathFragment);
}


ShaderDeferredLight::~ShaderDeferredLight() {
}

void ShaderDeferredLight::SetLightUniforms(BaseLight* light_, int lightIdx_) {
	SetVec3("lights[" + std::to_string(lightIdx_) + "].Position", light_->GetPosition());
	SetVec3("lights[" + std::to_string(lightIdx_) + "].Color", light_->GetColor());
}
