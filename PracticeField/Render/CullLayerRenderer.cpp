#include "CullLayerRenderer.h"

#include "../FilePooler.h"
#include "../Bases/BasesBundle.h"
#include "../Render/Shader.h"

CullLayerRenderer::CullLayerRenderer() : Renderer() {
}


CullLayerRenderer::~CullLayerRenderer() {
}

void CullLayerRenderer::SetShader() {
	Renderer::SetShader(FilePooler::LoadShader(VS_LayerCull, FS_LayerCulled));
}

void CullLayerRenderer::SetAdditionalShaderData(Shader * shader_) {
	id_refViewPos = shader_->GetUniformLocation("refViewPos");
}

void CullLayerRenderer::SetUniformAdditional() {
	glUniform3f(id_refViewPos, 
		refTransform->position.x, refTransform->position.y, refTransform->position.z);
}
