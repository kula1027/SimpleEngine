#include "RenderPipeLine.h"


float RenderPipeLine::quadVertices[] = { // vertex attributes for a quad that fills the entire screen in Normalized Device Coordinates.
										 // positions   // texCoords
	-1.0f,  1.0f,  0.0f, 1.0f,
	-1.0f, -1.0f,  0.0f, 0.0f,
	1.0f, -1.0f,  1.0f, 0.0f,

	-1.0f,  1.0f,  0.0f, 1.0f,
	1.0f, -1.0f,  1.0f, 0.0f,
	1.0f,  1.0f,  1.0f, 1.0f
};

int RenderPipeLine::quadSize = 24;


RenderPipeLine::RenderPipeLine()
{
}


RenderPipeLine::~RenderPipeLine()
{
}
