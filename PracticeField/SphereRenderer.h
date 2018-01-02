#pragma once

#include "Renderer.h"

class ImaginarySphere;
class ImaginaryPlane;
class ImaginaryDisk;

class SphereRenderer : public Renderer
{
private:
	int vertDivision;
	int horiDivision;

	void CalculateBoudingSphere();
	ImaginaryPlane* CalcCuttingPlane(glm::vec3 dirCam_);	
		
public:
	
	SphereRenderer();
	~SphereRenderer();

	Transform* fakeCamTr;
	ImaginarySphere* boundingSphere;
	int** idxPosition;
	ImaginaryDisk** dividedMeshDisks;

	virtual void SetMeshModel(MeshModel* meshModel_);
	virtual void Render(Camera* cam_, std::vector<BaseLight*> lights_);
};

struct AnglePosition {
	float dotValue;
	int idx;
};
