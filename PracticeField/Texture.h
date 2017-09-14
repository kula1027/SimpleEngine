#pragma once

#include <gl/glew.h>
#include <iostream>

using namespace std;

typedef enum TextureType {
	TextureType_Diffuse,
	TextureType_Specular,
	TextureType_Normals
};

class Texture
{
private:
	string path;

	int width, height;
	TextureType type;

	int refCount;

	unsigned char* imageData;

public:
	GLuint id;
	string typeName;

	Texture();
	Texture(string path_, TextureType type_);
	~Texture();

	void SetParams(GLenum paramName_, GLuint param_);
	string GetDirectory();
};

