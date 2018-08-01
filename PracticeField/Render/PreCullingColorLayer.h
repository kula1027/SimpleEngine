#pragma once


#include "PreCullingRenderer_Split.h"

class ImaginarySphere;
class ImaginaryPlane;
class ImaginaryDisk;


class PreCullingColorLayer : public PreCullingRenderer_Split {
private:

public:
	PreCullingColorLayer();
	~PreCullingColorLayer();

	/*
	Transform* targetCamTr;
	ImaginarySphere* boundingSphere;
	int** idxPosition;
	ImaginaryDisk** dividedMeshDisks;*/

	void ColorTriangles(glm::vec3 color_, Mesh * mesh_, int from, int count);

	void Render(RenderData* renderData_);
};
