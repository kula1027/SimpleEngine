#pragma once
#include "Renderer.h"
#include <vector>

#define DefaultVS_Outline "outline.vert"
#define DefaultFS_Outline "outline.frag"

struct Outline {
	bool draw = true;
	float thickness = 0.08f;
	int id_thickness;
	int id_color;
	glm::vec3 color = glm::vec3(0.2, 0.5, 1.0);

};

class OutlineRenderer :
	public Renderer
{
private:
	Shader * outlineShader = NULL;

protected:
	void SetAdditionalShaderData(Shader* shader_);

public:
	OutlineRenderer();
	~OutlineRenderer();
	
	void Render(Camera* cam_, std::vector<BaseLight*> lights_);

	Outline outline;
};

