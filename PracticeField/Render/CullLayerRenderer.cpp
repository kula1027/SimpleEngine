#include "CullLayerRenderer.h"

#include "../FileManager.h"
#include "../Render/Shader.h"

CullLayerRenderer::CullLayerRenderer() : Renderer() {
}


CullLayerRenderer::~CullLayerRenderer() {
}

void CullLayerRenderer::SetShader() {
	Renderer::SetShader(FileManager::LoadShader(VS_LayerCull, FS_LayerCulled));
}

void CullLayerRenderer::SetAdditionalShaderData(Shader * shader_) {
	id_refViewPos = shader_->GetUniformLocation("refViewPos");
}

void CullLayerRenderer::SetUniformAdditional() {

}
