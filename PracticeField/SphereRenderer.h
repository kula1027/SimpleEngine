#pragma once

#include "Renderer.h"

class SphereRenderer : public Renderer
{
private:
	const static int vertDivision = 8;

	void CalculateBoudingSphere();
	float GetHorizontalAngleRange(glm::vec3 dirCam_);
	float boundingRadius;
	glm::vec3 boundingCenter;

	float radiusVert;
	float radiusHori;

	static int** idxPosition;

public:
	const static int horiDivision = 8;
	SphereRenderer();
	~SphereRenderer();

	float GetBoundingRadius();
	glm::vec3 GetBoundingCenter();
	virtual void SetMeshModel(MeshModel* meshModel_);
	virtual void Render(Camera* cam_, std::vector<BaseLight*> lights_);
};

struct AnglePosition {
	float dotValue;
	int idx;
};
