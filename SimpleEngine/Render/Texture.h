#pragma once

#include <gl/glew.h>
#include <iostream>

using namespace std;

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

	bool hasAlphaTransparency;

	Texture();
	Texture(string path_, bool hasAlphaTransparency_ = false);
	~Texture();

	void SetParams(GLenum paramName_, GLuint param_);
	string GetDirectory();
};

