#include "RP_Deferred.h"

#include "../Scenes/SceneIncludes.h"
#include <GameWindow.h>


#include <Shaders/ShaderManager.h>
#include <Lights/LightManager.h>

void RP_Deferred::SetupFrameBuffers() {

	glGenFramebuffers(1, &gBuffer);
	glBindFramebuffer(GL_FRAMEBUFFER, gBuffer);

	//position buffer
	glGenTextures(1, &gPosition);
	glBindTexture(GL_TEXTURE_2D, gPosition);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, GameWindow::GetWidth(), GameWindow::GetHeight(), 0, GL_RGB, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, gPosition, 0);//texture를 fb에 attach한다.

	//normal buffer
	glGenTextures(1, &gNormal);
	glBindTexture(GL_TEXTURE_2D, gNormal);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, GameWindow::GetWidth(), GameWindow::GetHeight(), 0, GL_RGB, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D, gNormal, 0);

	//color + specular buffer
	glGenTextures(1, &gAlbedoSpec);
	glBindTexture(GL_TEXTURE_2D, gAlbedoSpec);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, GameWindow::GetWidth(), GameWindow::GetHeight(), 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT2, GL_TEXTURE_2D, gAlbedoSpec, 0);

	attachments[0] = GL_COLOR_ATTACHMENT0;
	attachments[1] = GL_COLOR_ATTACHMENT1;
	attachments[2] = GL_COLOR_ATTACHMENT2;

	glDrawBuffers(3, attachments);//Draw할 Attachements를 지정

	unsigned int rboDepth;
	glGenRenderbuffers(1, &rboDepth);
	glBindRenderbuffer(GL_RENDERBUFFER, rboDepth);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, GameWindow::GetWidth(), GameWindow::GetHeight());
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, rboDepth);
	// finally check if framebuffer is complete
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		std::cout << "Framebuffer not complete!" << std::endl;
}

void RP_Deferred::SetupShaders() {
	//Shader Setup
	shaderGeo = new BaseShader("Deferred/deferred_geo");
	shaderDirectional = new BaseShader("Deferred/deferred_light_directional");	
	shaderPointStencil = new BaseShader("Deferred/deferred_light_pointStencil");

	shaderDirectional->Use();
	shaderDirectional->BindLightUBO();
	shaderDirectional->SetInt("gPosition", 0);
	shaderDirectional->SetInt("gNormal", 1);
	shaderDirectional->SetInt("gAlbedoSpec", 2);
}

void RP_Deferred::GeometryPass(SceneRenderData* sceneRenderData_) {
#ifdef DEBUG
	glClearColor(0.3f, 0.3f, 0.3f, 1.0f);
#endif

	int rdrCount_Deferred = sceneRenderData_->renderQueue_Deferred.size();

	glBindFramebuffer(GL_FRAMEBUFFER, gBuffer);	
	
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	shaderGeo->Use();
	for (int loop = 0; loop < rdrCount_Deferred; loop++) {
		shaderGeo->SetMat_M(sceneRenderData_->renderQueue_Deferred[loop]->Mmatrix());
		sceneRenderData_->renderQueue_Deferred[loop]->RenderMesh();
	}
}

void RP_Deferred::LightPass() {
#ifdef DEBUG
	glClearColor(0, 0.3f, 0.3f, 1.0f);
#endif
	glBindFramebuffer(GL_FRAMEBUFFER, offScreenData.frameBuffer);	
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	LightPass_AmbientDirectional();

	CopyFboDepth(gBuffer, offScreenData.frameBuffer);

	LightPass_Point();
	//LightPass Directional
	//glDisable(GL_DEPTH_TEST);
	//glEnable(GL_BLEND);
	//glBlendEquation(GL_FUNC_ADD);
	//glBlendFunc(GL_ONE, GL_ONE);	
	//
	//glDisable(GL_BLEND);
	//glEnable(GL_DEPTH_TEST);


}

void RP_Deferred::LightPass_AmbientDirectional() {
	shaderDirectional->Use();
	glActiveTexture(GL_TEXTURE0);//다음 bind tex는 GL_TEXTURE0에 대해서.
	glBindTexture(GL_TEXTURE_2D, gPosition);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, gNormal);
	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, gAlbedoSpec);
	shaderDirectional->SetVec3("viewPos", targetCamera->transform->position);
	DrawOffScreenQuad();
}

void RP_Deferred::LightPass_Point() {
	//stencil pass

	
	
	std::map<unsigned int, PointLight*>* mapPointLights = &(LightManager::Inst()->pointLights);
	std::map<unsigned int, PointLight*>::iterator it;
	for (it = mapPointLights->begin(); it != mapPointLights->end(); it++) {		
		shaderPointStencil->Use();

		glEnable(GL_DEPTH_TEST);	

		glDisable(GL_CULL_FACE);

		glClear(GL_STENCIL_BUFFER_BIT);		
		glStencilFunc(GL_ALWAYS, 0, 0);//always pass stencil

		////stencil fail / stencil pass, depth fail / both fail
		////stencil은 항상 pass 하기에 sdfail만이 의미있다.
		glStencilOpSeparate(GL_BACK, GL_KEEP, GL_INCR_WRAP, GL_KEEP);
		glStencilOpSeparate(GL_FRONT, GL_KEEP, GL_DECR_WRAP, GL_KEEP);

		mat4 mvpSphere = targetCamera->VPmatrix() * it->second->GetModelMatrix();		
		shaderPointStencil->SetMat_MVP(mvpSphere);
		glBindVertexArray(m->VAO);
		glDrawElements(
			GL_TRIANGLES,
			m->GetIdxCount(),
			GL_UNSIGNED_INT,
			NULL
		);
	}	
	
	//p.WorldPos(m_pointLight[PointLightIndex].Position);

	//float BBoxScale = CalcPointLightBSphere(m_pointLight[PointLightIndex].Color,
	//	m_pointLight[PointLightIndex].DiffuseIntensity);
	//p.Scale(BBoxScale, BBoxScale, BBoxScale);
	//p.SetCamera(m_pGameCamera->GetPos(), m_pGameCamera->GetTarget(), m_pGameCamera->GetUp());
	//p.SetPerspectiveProj(m_persProjInfo);

	//m_nullTech.SetWVP(p.GetWVPTrans());
	//m_bsphere.Render();
}

void RP_Deferred::AdditionalForwardPass(SceneRenderData* sceneRenderData_) {
	int rdrCount_Forward = sceneRenderData_->renderQueue_Forward.size();
	
	for (int loop = 0; loop < rdrCount_Forward; loop++) {
		sceneRenderData_->renderQueue_Forward[loop]->RenderMesh_Forward(targetCamera);
	}

}

RP_Deferred::RP_Deferred() {
	InitOffScreenDraw();

	SetupFrameBuffers();

	SetupShaders();

	m = FilePooler::LoadMeshModel("sphere.obj")->meshes->at(0);
}


RP_Deferred::~RP_Deferred() {
}

void RP_Deferred::Render(SceneRenderData* sceneRenderData_) {
	int rdrCount_Deferred = sceneRenderData_->renderQueue_Deferred.size();
	int rdrCount_Forward = sceneRenderData_->renderQueue_Forward.size();	
	
	targetCamera->SetUpMatrices();
	for (int loop = 0; loop < rdrCount_Deferred; loop++) {
		sceneRenderData_->renderQueue_Deferred[loop]->ComputeMatrices(targetCamera);
	}
	for (int loop = 0; loop < rdrCount_Forward; loop++) {
		sceneRenderData_->renderQueue_Forward[loop]->ComputeMatrices(targetCamera);
	}

	//Geometry Pass		
	GeometryPass(sceneRenderData_);
	//mainCamera_->RenderSkyBox();

	//Lighting Pass
	LightPass();

	//Additional Forward
	AdditionalForwardPass(sceneRenderData_);

	//Render off rendered frame to default fb
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glDisable(GL_DEPTH_TEST);
	glClearColor(0.0f, 0.7f, 0.2f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);
	offScreenData.screenShader->Use();
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, offScreenData.texColorBuffer);
	DrawOffScreenQuad();

	glEnable(GL_DEPTH_TEST);
}
