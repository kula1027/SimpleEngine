#include "Texture.h"
#include <SOIL\SOIL.h>


Texture::Texture(){
	glGenTextures(1, &textureId);
}

Texture::Texture(string path_, TextureType type_) {
	type = type_;
	switch (type) {
	case TextureType_Diffuse:
		typeName = "texture_diffuse";
		break;
	case TextureType_DiffuseTransparent:
		typeName = "texture_diffuse";
		break;
	case TextureType_Specular:
		typeName = "texture_specular";
		break;
	case TextureType_Normals:
		typeName = "texture_normal";
		break;
	
	}
	path = path_;

	glGenTextures(1, &textureId);	

	if (type != TextureType_DiffuseTransparent) {
		imageData = SOIL_load_image(path_.c_str(), &width, &height, 0, SOIL_LOAD_RGB);
		glBindTexture(GL_TEXTURE_2D, textureId);

		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, imageData);
		glGenerateMipmap(GL_TEXTURE_2D);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	}else {
		imageData = SOIL_load_image(path_.c_str(), &width, &height, 0, SOIL_LOAD_RGBA);
		glBindTexture(GL_TEXTURE_2D, textureId);

		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, imageData);
		glGenerateMipmap(GL_TEXTURE_2D);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	}

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glBindTexture(GL_TEXTURE_2D, 0);

	SOIL_free_image_data(imageData);
}

Texture::~Texture()
{
}

void Texture::SetParams(GLenum paramName_, GLuint param_){
	glBindTexture(GL_TEXTURE_2D, textureId);
	glTexParameteri(GL_TEXTURE_2D, paramName_, param_);
	glBindTexture(GL_TEXTURE_2D, 0);
}

string Texture::GetDirectory(){
	return path;
}
