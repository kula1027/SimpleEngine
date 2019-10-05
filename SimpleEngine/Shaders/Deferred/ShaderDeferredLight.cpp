#include "ShaderDeferredLight.h"
#include <string.h>
#include <vector>

#include <Lights/LightsBundle.h>
#include <Bases/Transform.h>

ShaderDeferredLight::ShaderDeferredLight() {
	filePath = "Deferred/deferred_light";

	LoadProgram(filePath);

	BindLightUBO();
}


ShaderDeferredLight::~ShaderDeferredLight() {
}