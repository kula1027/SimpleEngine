#pragma once


#define DEBUG //comment this to disable log


#pragma region Shader

#define VertexShaderFormat ".vert"
#define FragmentShaderFormat ".frag"
#define GeometryShaderFormat ".geom"
#define DirPathShader "../Shaders/"

#pragma endregion

#define DirPathMaterial "../Materials/"

enum VsyncMode {
	VSYNC_OFF,
	VSYNC_ON
};