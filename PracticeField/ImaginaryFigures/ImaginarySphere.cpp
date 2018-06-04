#include "ImaginarySphere.h"

#include "../Mesh/Mesh.h"
#include "../Render/Renderer.h"
#include "../Bases/Camera.h"
#include "../FileManager.h"

ImaginarySphere::ImaginarySphere() {
}

ImaginarySphere::ImaginarySphere(vec3 center_, float radius_) {
	center = center_;
	radius = radius_;
}


ImaginarySphere::~ImaginarySphere() {
}

void ImaginarySphere::Render(Camera* cam, std::vector<BaseLight*> lights) {
	if (renderer == NULL) {
		renderer = new Renderer();
		renderer->SetMeshModel(FileManager::LoadMeshModel("sphere.obj"));
		renderer->SetDefaultShader();
		renderer->castShadow = false;
		renderer->cullingEnabled = false;
		renderer->lineDrawEnabled = true;

		Transform* tr = new Transform();
		tr->position = center;
		tr->scale = glm::vec3(radius);
		renderer->SetTransform(tr);
	}

	renderer->Render(cam, lights);
}

ImaginarySphere * ImaginarySphere::GetBoundingSphere(Mesh * mesh, glm::vec3 center_) {
	float largestDist = -1;	
	for (int loop = 0; loop < mesh->vertices.size(); loop++) {
		float dist = glm::distance(center_, mesh->vertices[loop].position);

		if (dist > largestDist) {
			largestDist = dist;
		}		
	}

	return new ImaginarySphere(center_, largestDist);
}
