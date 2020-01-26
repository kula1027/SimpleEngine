#include "Renderer_Forward.h"

#include <Mesh/MeshModel.h>

#include <Shaders/ShaderManager.h>
#include <Shaders/ShaderBundle.h>
#include <Render/RenderMaterial/RenderMaterial.h>
#include <Light/LightManager.h>
#include <Light/LightsBundle.h>

Renderer_Forward::Renderer_Forward() {
	renderType = RenderType_Forward;
	shader = ShaderManager::GetShader("Forward/forward"); 	
}


Renderer_Forward::~Renderer_Forward() {
}

void Renderer_Forward::RenderMesh() {
	shader->SetUniforms(this);
	int dLightCount = LightManager::Inst()->directionalLights.size();
	for (int loop = 0; loop < dLightCount; loop++) {		
		if (LightManager::Inst()->directionalLights[loop]->castShadow) {
			LightManager::Inst()->directionalLights[loop]->BindShadowMap();
		}
	}

	MeshRenderer::RenderMesh();
}
