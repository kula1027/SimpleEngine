#pragma once

#include "Renderer.h"

class SphereRenderer : public Renderer
{
private:
	const int divisionCount = 10;

	void CalculateBoudingSphere();
	float boundingRadius;
	glm::vec3 boundingCenter;

public:
	SphereRenderer();
	~SphereRenderer();

	float GetBoundingRadius();
	glm::vec3 GetBoundingCenter();
	virtual void SetMeshModel(MeshModel* meshModel_);
	virtual void Render(Camera* cam_, std::vector<BaseLight*> lights_);
};

