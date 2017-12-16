#pragma once

#include "Renderer.h"

class ImaginarySphere;

class SphereRenderer : public Renderer
{
private:
	const static int vertDivision = 8;

	void CalculateBoudingSphere();
	float GetTangentLines(glm::vec3 dirCam_);

	ImaginarySphere* boundingSphere;

	static int** idxPosition;

public:
	const static int horiDivision = 8;
	SphereRenderer();
	~SphereRenderer();

	virtual void SetMeshModel(MeshModel* meshModel_);
	virtual void Render(Camera* cam_, std::vector<BaseLight*> lights_);
};

struct AnglePosition {
	float dotValue;
	int idx;
};
