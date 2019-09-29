#pragma once

class Texture;

class RenderMaterial {
public:
	RenderMaterial();
	~RenderMaterial();

	Texture* texDiffuse;
	Texture* texSpec;
	Texture* texNormal;

	void ApplyTexture();
	void SetUniforms();
};

