#pragma once

#include <gl/glew.h>
#include <glm\glm.hpp>
#include <vector>

#define DefualtVS "default.vert"
#define DefaultFS "default.frag"

#define AttrLoc_IstMatrix0 3
#define AttrLoc_IstMatrix1 4
#define AttrLoc_IstMatrix2 5
#define AttrLoc_IstMatrix3 6

struct ID_matrice {
	GLuint mvp = -1;
	GLuint view = -1;
	GLuint model = -1;
	GLuint vp = -1;
};

struct ID_dLight {
	GLuint direction;
	GLuint color;
	GLuint power;
	GLuint lightSpaceMatrix;
	GLuint shadowMap;
};

struct ID_pLight {
	GLuint position;
	GLuint color;
	GLuint power;
};

struct ID_diffuseTexture {
	GLuint count;
	GLuint id;
};

struct ID_specularTexture {
	GLuint count;
	GLuint id;
};



class Shader;
class Transform;
class Camera;
class MeshModel;
class BaseLight;
class Mesh;
class MeshModel;

class Renderer{
protected:
	Shader* shader = NULL;
	
	ID_matrice id_matrice;
	ID_dLight id_dLight;
	ID_pLight id_pLight;
	ID_diffuseTexture id_diffuse;
	ID_specularTexture id_specular;

	glm::mat4 modelMatrix;
	glm::mat4 mvpMatrix;

	Transform* transform = NULL;//ref to gameObject transform
	MeshModel* meshModel = NULL;

	void ApplyTexture(Mesh* processingMesh_);

	void SetDrawingMode();
	void SetUniformDlight(Camera* cam_, BaseLight* dLight);
	void SetUniformMVP(Camera* cam_);	
	void RestoreDrawingMode();

	virtual void SetAdditionalShaderData(Shader* shader_) {}
	virtual void SetUniformAdditional() {}

public:	
	Renderer();
	Renderer(MeshModel* meshModel_);
	Renderer(Transform* transform_);
	~Renderer();

	void SetTransform(Transform* transform_);
	void ComputeMatrix();
	void RenderShadowMap(BaseLight* light_);
	void SetupIdx();

	MeshModel* GetMeshModel();

	virtual void SetShader(Shader* shader_);
	virtual void SetShader();
	virtual void Render(Camera* cam, std::vector<BaseLight*> lights);
	virtual void SetMeshModel(MeshModel* meshModel_);

	bool castShadow;
	bool cullingEnabled;
	bool lineDrawEnabled;
};
