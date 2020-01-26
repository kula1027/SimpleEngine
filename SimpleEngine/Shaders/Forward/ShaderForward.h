#pragma once
#include <Shaders/BaseShader.h>

class Camera;
class BaseLight;
class Mesh;

class ShaderForward : public BaseShader {
public:
	ShaderForward();	
	~ShaderForward();

	ID_matrice id_matrice;
	
	ID_diffuseTexture id_diffuse;
	ID_specularTexture id_specular;

	virtual void SetUniforms(MeshRenderer* renderer_) override;		
};

