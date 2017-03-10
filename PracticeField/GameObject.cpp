#include "GameObject.h"

void GameObject::ComputeModelMatrix(Camera* cam) {
	modelMatrix = glm::translate(glm::mat4(1.0), transform.position);
	mvpMatrix = cam->VPmatrix() * modelMatrix;
}

GameObject::GameObject(){
}

void GameObject::Render(Camera* cam, std::vector<Light*> lights_){
	ComputeModelMatrix(cam);

	glUseProgram(shader->GetID());

	glUniformMatrix4fv(mvpMatrixID, 1, GL_FALSE, glm::value_ptr(mvpMatrix));
	glUniformMatrix4fv(viewMatrixID, 1, GL_FALSE, glm::value_ptr(cam->Vmatrix()));
	glUniformMatrix4fv(modelMatrixID, 1, GL_FALSE, glm::value_ptr(modelMatrix));

	glUniform3f(lightPosID, lights_[0]->position.x, lights_[0]->position.y, lights_[0]->position.z);
	glUniform3f(lightColorID, lights_[0]->color.x, lights_[0]->color.y, lights_[0]->color.z);
	glUniform1f(lightPowerID, lights_[0]->intensity);

	meshModel->Draw(shader);
}

void GameObject::Update(){			
	//transform.position.x += Time::deltaTime;
}


void GameObject::SetModel(MeshModel * meshModel_) {
	meshModel = meshModel_;
}

void GameObject::SetShader(Shader * shader_) {
	shader = shader_;
	mvpMatrixID = glGetUniformLocation(shader->GetID(), "MVP");
	viewMatrixID = glGetUniformLocation(shader->GetID(), "V");
	modelMatrixID = glGetUniformLocation(shader->GetID(), "M");
	lightPosID = glGetUniformLocation(shader->GetID(), "LightPosition_worldspace");
	lightColorID = glGetUniformLocation(shader->GetID(), "LightColor");
	lightPowerID = glGetUniformLocation(shader->GetID(), "LightPower");
}


GameObject::~GameObject(){	
}
