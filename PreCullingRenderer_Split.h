#pragma once

#include "Renderer.h"

class ImaginarySphere;
class ImaginaryPlane;
class ImaginaryDisk;


struct PcRenderMaterial {
	Transform* targetCamTr;
	ImaginarySphere* boundingSphere;
	int** idxPosition;
	ImaginaryDisk** dividedMeshDisks;

	int vertDivision = 4;
	int horiDivision = 4;
};


class PreCullingRenderer_Split : public Renderer
{
private:
	/*int vertDivision;
	int horiDivision;*/

	void CalculateBoudingSphere();
	ImaginaryPlane* CalcCuttingPlane(glm::vec3 dirCam_);
	int CalcFrontFaceCount(Mesh * mesh_, glm::vec3 vPosition);	
		
public:	
	PreCullingRenderer_Split();
	~PreCullingRenderer_Split();
/*
	Transform* targetCamTr;
	ImaginarySphere* boundingSphere;
	int** idxPosition;
	ImaginaryDisk** dividedMeshDisks;*/

	PcRenderMaterial* renderMaterial;

	virtual void SetMeshModel(MeshModel* meshModel_);
	virtual void Render(Camera* cam_, std::vector<BaseLight*> lights_);
};

