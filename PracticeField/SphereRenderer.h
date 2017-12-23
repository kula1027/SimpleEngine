#pragma once

#include "Renderer.h"

class ImaginarySphere;
class ImaginaryPlane;
class ImaginaryDisk;

class SphereRenderer : public Renderer
{
private:
	const static int vertDivision = 8;

	void CalculateBoudingSphere();
	ImaginaryPlane* CalcCuttingPlane(glm::vec3 dirCam_);

	ImaginaryDisk** dividedMeshDisks;
		

	static int** idxPosition;

public:
	const static int horiDivision = 8;
	SphereRenderer();
	~SphereRenderer();

	Transform* fakeCamTr;
	ImaginarySphere* boundingSphere;

	virtual void SetMeshModel(MeshModel* meshModel_);
	virtual void Render(Camera* cam_, std::vector<BaseLight*> lights_);
};

struct AnglePosition {
	float dotValue;
	int idx;
};
