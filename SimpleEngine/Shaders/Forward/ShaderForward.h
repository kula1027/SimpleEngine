#pragma once
#include <Shaders/BaseShader.h>

class Camera;
class BaseLight;
class Mesh;

class ShaderForward : public BaseShader {
public:
	ShaderForward();
	ShaderForward(string vs, string fs);
	~ShaderForward();

	ID_matrice id_matrice;
	ID_dLight id_dLight;
	ID_pLight id_pLight;
	ID_diffuseTexture id_diffuse;
	ID_specularTexture id_specular;

	virtual void Initialize();	

	virtual void SetUniforms(
		Camera* camera_, MeshRenderer* renderer_, std::vector<BaseLight*>* lights_) override;	

	void SetUniformDlight(Camera* cam_, BaseLight* dLight_);
//	void SetUniformMVP(RenderData* renderData_, glm::mat4 modelMat_, glm::mat4 mvpMat_);	
};

