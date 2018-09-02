#pragma once
#include "BaseShader.h"

class Camera;
class BaseLight;
class Mesh;

class DefaultShader : public BaseShader {
public:
	DefaultShader();
	DefaultShader(string vs, string fs);
	~DefaultShader();

	ID_matrice id_matrice;
	ID_dLight id_dLight;
	ID_pLight id_pLight;
	ID_diffuseTexture id_diffuse;
	ID_specularTexture id_specular;

	virtual void Initialize();	

	virtual void SetUniforms(RenderData* renderData_, glm::mat4 modelMat_, glm::mat4 mvpMat_);
	virtual void ApplyTexture(Mesh* mesh_);

	void SetUniformDlight(Camera* cam_, BaseLight* dLight_);
	void SetUniformMVP(RenderData* renderData_, glm::mat4 modelMat_, glm::mat4 mvpMat_);	
};

