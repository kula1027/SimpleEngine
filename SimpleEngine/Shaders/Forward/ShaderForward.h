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

	virtual void SetUniforms(MeshRenderer* renderer_) override;		
};

