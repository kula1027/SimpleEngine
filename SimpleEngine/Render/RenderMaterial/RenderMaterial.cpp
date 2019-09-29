#include "RenderMaterial.h"

#include <gl/glew.h>
#include <Render/Texture.h>

RenderMaterial::RenderMaterial() {
}


RenderMaterial::~RenderMaterial() {
}

void RenderMaterial::ApplyTexture() {

	//Diffuse
	if (texDiffuse != NULL) {		
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texDiffuse->textureId);
	}
	

	//spec
	if (texSpec != NULL) {
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, texSpec->textureId);
	}

	//normal
	if (texNormal != NULL) {
		glActiveTexture(GL_TEXTURE2);
		glBindTexture(GL_TEXTURE_2D, texNormal->textureId);
	}
}
