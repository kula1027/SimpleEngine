#pragma once
class MeshMaterial
{
public:
	MeshMaterial();
	~MeshMaterial();

	void LoadShaders();
	void GetShader(const char* shaderName);
};

