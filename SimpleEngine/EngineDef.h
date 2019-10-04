#pragma once


//RenderMode
#define RenderMode_Forward 0
#define RenderMode_Deferred 1
#define RenderMode_SimpleSingle 2


#define VsyncMode 1 // 1-> vsync on, 0 -> off

#define DEBUGMODE //comment this to disable log

#define DefaultRenderMode RenderMode_Deferred


#pragma region Shader

#define VertexShaderFormat ".vert"
#define FragmentShaderFormat ".frag"
#define GeometryShaderFormat ".geo"
#define DirPathShader "../Shaders/"
#define BindingPointLightData 1

#pragma endregion

#define DirPathMaterial "../Materials/"