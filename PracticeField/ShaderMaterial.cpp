#include "ShaderMaterial.h"



ShaderMaterial::ShaderMaterial(){
	shaderId = 
		LoadShaders(
			"SimpleShader.shader"
			);
}

ShaderMaterial::~ShaderMaterial(){
}
