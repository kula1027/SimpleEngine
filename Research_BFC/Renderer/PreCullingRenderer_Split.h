#pragma once


#include <Render/DefaultRenderer.h>

class ImaginarySphere;
class ImaginaryPlane;
class ImaginaryDisk;
class Texture;


struct PcRenderMaterial {
	Transform* targetCamTr;
	ImaginarySphere* boundingSphere;
	int** idxPosition;
	ImaginaryDisk** dividedMeshDisks;
	std::vector<Texture*> textures;

	int vertDivision = 4;
	int horiDivision = 4;
};


class PreCullingRenderer_Split : public DefaultRenderer
{
protected:
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
	virtual void Render(RenderData* renderData_);
};

