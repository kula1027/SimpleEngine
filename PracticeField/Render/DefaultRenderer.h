#pragma once

#include "BaseRenderer.h"

#define AttrLoc_IstMatrix0 3
#define AttrLoc_IstMatrix1 4
#define AttrLoc_IstMatrix2 5
#define AttrLoc_IstMatrix3 6





class DefaultRenderer : public BaseRenderer{
protected:
	
	
	ID_matrice id_matrice;
	ID_dLight id_dLight;
	ID_pLight id_pLight;
	ID_diffuseTexture id_diffuse;
	ID_specularTexture id_specular;		

	void ApplyTexture(Mesh* processingMesh_);

	void SetDrawingMode();
	void SetUniformDlight(Camera* cam_, BaseLight* dLight);
	void SetUniformMVP(Camera* cam_);	
	void RestoreDrawingMode();

	virtual void SetAdditionalShaderData(BaseShader* shader_) {}
	virtual void SetUniformAdditional() {}

public:	
	DefaultRenderer();
	DefaultRenderer(MeshModel* meshModel_);
	DefaultRenderer(Transform* transform_);
	~DefaultRenderer();

	virtual void RenderShadowMap(BaseLight* light_);



	virtual void SetShader(BaseShader* shader_);
	virtual void SetShader();
	virtual void Render(Camera* cam, std::vector<BaseLight*> lights);


};

