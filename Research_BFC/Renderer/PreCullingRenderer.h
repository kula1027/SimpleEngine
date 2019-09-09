#pragma once


#include <Render/DefaultRenderer.h>

class ImaginarySphere;
class ImaginaryPlane;
class ImaginaryDisk;

struct PcRendererMaterial {
	Transform* targetCamTr;
	ImaginarySphere* boundingSphere;
	int** idxPosition;
	ImaginaryDisk** dividedMeshDisks;

	int** verticalIdxer;//0번째 배열에 vertical face 시작 위치, 1번째 배열에 각 seg 갯수	

	int vertDivision = 6;
	int horiDivision = 64;
};


class PreCullingRenderer : public DefaultRenderer
{
private:
	/*int vertDivision;
	int horiDivision;*/

	void CalculateBoudingSphere();
	ImaginaryPlane* CalcCuttingPlane(glm::vec3 dirCam_);
	int CalcFrontFaceCount(Mesh * mesh_, glm::vec3 vPosition);	

public:
	PreCullingRenderer();
	~PreCullingRenderer();

	/*
	Transform* targetCamTr;
	ImaginarySphere* boundingSphere;
	int** idxPosition;
	ImaginaryDisk** dividedMeshDisks;*/

	PcRendererMaterial* renderMaterial;

	virtual void SetMeshModel(MeshModel* meshModel_);
	virtual void Render(RenderData* renderData_);
};

