#include "Renderer_Forward.h"

#include <Mesh/MeshModel.h>

#include <Shaders/ShaderManager.h>
#include <Shaders/ShaderBundle.h>
#include <Render/RenderMaterial/RenderMaterial.h>

Renderer_Forward::Renderer_Forward() {
	shader = ShaderManager::GetShader("Forward/forward");
}


Renderer_Forward::~Renderer_Forward() {
}

void Renderer_Forward::RenderMesh() {
	shader->SetUniforms(this);

	MeshRenderer::RenderMesh();
}
