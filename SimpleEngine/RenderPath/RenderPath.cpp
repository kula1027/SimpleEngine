#include "RenderPath.h"


float RenderPath::quadVertices[] = { // vertex attributes for a quad that fills the entire screen in Normalized Device Coordinates.
										 // positions   // texCoords
	-1.0f,  1.0f,  0.0f, 1.0f,
	-1.0f, -1.0f,  0.0f, 0.0f,
	1.0f, -1.0f,  1.0f, 0.0f,

	-1.0f,  1.0f,  0.0f, 1.0f,
	1.0f, -1.0f,  1.0f, 0.0f,
	1.0f,  1.0f,  1.0f, 1.0f
};

int RenderPath::quadSize = 24;


RenderPath::RenderPath()
{
}


RenderPath::~RenderPath()
{
}
