#include "Camera.h"

#include <gl\glew.h>
#include <Debugger/SP_Debugger.h>
#include "BasesBundle.h"
#include "../GameWindow.h"
#include <Shaders/BaseShader.h>
#include "../FilePooler.h"

#include <Render/RenderPath/RenderPathBundle.h>
#include <Shaders/ShaderDef.h>


void Camera::InitUbo() {
	glGenBuffers(1, &uboCamera);
	glBindBuffer(GL_UNIFORM_BUFFER, uboCamera);
	
	glBufferData(GL_UNIFORM_BUFFER, SizeStructCameraData, NULL, GL_STATIC_DRAW);
	glBindBufferBase(GL_UNIFORM_BUFFER, BindingPointCameraData, uboCamera);

	SetScreenWidthHeight(GameWindow::GetWidth(), GameWindow::GetHeight());
}

Camera::Camera(){
	DebugLog("Initialize Camera...");

	InitUbo();

	name = "Camera";	

	GetTransform()->SetPosition(glm::vec3(0, 0, 0));
	clearColor = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);

	fov = 1.0f;
	near = 0.3f;
	far = 1000.0f;

	normalizedViewPort.x = 1;
	normalizedViewPort.y = 1;	
	

	projMode = PROJECTION_PERSPECTIVE;
	switch (projMode) {
	case PROJECTION_PERSPECTIVE:
		projectionMatrix = glm::perspective(fov, (float)GameWindow::GetWidth() / (float)GameWindow::GetHeight(), near, far);
		break;

	case PROJECTION_ORTHO:
		//projectionMatrix = glm::ortho(fov, (float)GameWindow::GetWidth() / (float)GameWindow::GetHeight(), near, far);
		//TODO
		break;

	default:
		DebugError("Projection mode not defined.");		
		projectionMatrix = glm::perspective(fov, (float)GameWindow::GetWidth() / (float)GameWindow::GetHeight(), near, far);
		break;
	}

	renderPath = new RP_Deferred();	
	renderPath->SetTargetCamera(this);

	skybox = new EmptySkyBox();
	skybox->Initialize();
}

Camera::~Camera(){
}


void Camera::SetSkybox(SkyBox * skybox_) {
	if (skybox != NULL) {
		free(skybox);
	}

	skybox = skybox_;
	skybox->Initialize();
}

SkyBox * Camera::GetSkybox() {
	return skybox;
}

void Camera::SetScreenWidthHeight(float w, float h) {
	screenWidthHeight = glm::vec4(w, h, w, h);

	glBindBuffer(GL_UNIFORM_BUFFER, uboCamera);
	glBufferSubData(GL_UNIFORM_BUFFER, 224, sizeof(glm::vec4), &screenWidthHeight);
}

void Camera::Render(SceneRenderData* sceneRenderData_) {
	renderPath->Render(sceneRenderData_);
}

void Camera::ComputeMatrices(){
	upVector = glm::cross(GetTransform()->GetRight(), GetTransform()->GetForward());

	viewMatrix = glm::lookAt(
		GetTransform()->GetPosition(),
		GetTransform()->GetPosition() + GetTransform()->GetForward(),
		upVector                 
		);	

	vpMatrix = projectionMatrix * viewMatrix;	
	
	glBindBuffer(GL_UNIFORM_BUFFER, uboCamera);	
	glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(glm::mat4), &viewMatrix);//View
	glBufferSubData(GL_UNIFORM_BUFFER, sizeof(glm::mat4), sizeof(glm::mat4), &projectionMatrix);//Projection
	glBufferSubData(GL_UNIFORM_BUFFER, sizeof(glm::mat4) * 2, sizeof(glm::mat4), &vpMatrix);//P x V	
	glBufferSubData(GL_UNIFORM_BUFFER, sizeof(glm::mat4) * 3, sizeof(glm::vec3), &GetTransform()->GetPosition());//position
	glBufferSubData(GL_UNIFORM_BUFFER, sizeof(glm::mat4) * 3 + sizeof(glm::vec4), sizeof(glm::vec3), &GetTransform()->GetForward());//direction	
}

glm::mat4 Camera::VPmatrix() {
	return vpMatrix;
}

glm::mat4 Camera::Vmatrix() {
	return viewMatrix;
}

glm::mat4 Camera::Pmatrix() {
	return projectionMatrix;
}

void Camera::RenderSkyBox(){
	skybox->Render(this);
}

