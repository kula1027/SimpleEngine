#pragma once

#include <gl/glew.h>
#include <iostream>

using namespace std;

typedef enum TextureType {
	TextureType_Diffuse,
	TextureType_DiffuseTransparent,
	TextureType_Specular,
	TextureType_Normals
};

class Texture
{
private:
	string path;

	int width, height;
	

	int refCount;

	unsigned char* imageData;

public:
	GLuint textureId;
	string typeName;
	TextureType type;

	Texture();
	Texture(string path_, TextureType type_);
	~Texture();

	void SetParams(GLenum paramName_, GLuint param_);
	string GetDirectory();
};

