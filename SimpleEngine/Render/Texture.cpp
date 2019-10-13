#include "Texture.h"
#include <SOIL\SOIL.h>

#include <Debugger/SP_Debugger.h>

Texture::Texture(){
	glGenTextures(1, &textureId);
}

Texture::Texture(string path_, bool hasAlphaTransparency_ ) {
	DebugLog("Load Texture: " + path_);

	path = path_;

	glGenTextures(1, &textureId);	

	string filePath = DirPathMaterial + path_;

	if (hasAlphaTransparency) {		
		imageData = SOIL_load_image(filePath.c_str(), &width, &height, 0, SOIL_LOAD_RGBA);	

		glBindTexture(GL_TEXTURE_2D, textureId);

		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, imageData);
	}else {
		imageData = SOIL_load_image(filePath.c_str(), &width, &height, 0, SOIL_LOAD_RGB);		

		glBindTexture(GL_TEXTURE_2D, textureId);

		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, imageData);
	}
	
	glGenerateMipmap(GL_TEXTURE_2D);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glBindTexture(GL_TEXTURE_2D, 0);

	SOIL_free_image_data(imageData);
}

Texture::~Texture() {
}

void Texture::SetParams(GLenum paramName_, GLuint param_){
	glBindTexture(GL_TEXTURE_2D, textureId);
	glTexParameteri(GL_TEXTURE_2D, paramName_, param_);	
}

string Texture::GetDirectory(){
	return path;
}
