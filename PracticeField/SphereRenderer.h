#pragma once

#include "Renderer.h"

class ImaginarySphere;
class ImaginaryPlane;
class ImaginaryDisk;


struct SphereRenderMaterial {
	Transform* targetCamTr;
	ImaginarySphere* boundingSphere;
	int** idxPosition;
	ImaginaryDisk** dividedMeshDisks;

	int vertDivision = 64;
	int horiDivision = 16;
};


class SphereRenderer : public Renderer
{
private:
	/*int vertDivision;
	int horiDivision;*/

	void CalculateBoudingSphere();
	ImaginaryPlane* CalcCuttingPlane(glm::vec3 dirCam_);	
		
public:	
	SphereRenderer();
	~SphereRenderer();
/*
	Transform* targetCamTr;
	ImaginarySphere* boundingSphere;
	int** idxPosition;
	ImaginaryDisk** dividedMeshDisks;*/

	SphereRenderMaterial* renderMaterial;

	virtual void SetMeshModel(MeshModel* meshModel_);
	virtual void Render(Camera* cam_, std::vector<BaseLight*> lights_);
};

