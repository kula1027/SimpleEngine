#include "Skybox.h"

#include <SOIL\SOIL.h>
#include "../FilePooler.h"
#include "../Render/RenderBundle.h"
#include "../Bases/Camera.h"

#include <glm/gtc/type_ptr.hpp>



SkyBox::SkyBox() {

}


SkyBox::~SkyBox() {
}

void SkyBox::InitCubeMap() {
	facePath[SkyBox_Right] = DirPathSkyBox + string("right.jpg");
	facePath[SkyBox_Left] = DirPathSkyBox + string("left.jpg");
	facePath[SkyBox_Top] = DirPathSkyBox + string("top.jpg");
	facePath[SkyBox_Bottom] = DirPathSkyBox + string("bottom.jpg");
	facePath[SkyBox_Back] = DirPathSkyBox + string("back.jpg");
	facePath[SkyBox_Front] = DirPathSkyBox + string("front.jpg");

	glGenTextures(1, &cubeMapId);
	glBindTexture(GL_TEXTURE_CUBE_MAP, cubeMapId);

	int width, height;
	for (unsigned int i = 0; i < 6; i++) {
		unsigned char *imageData = SOIL_load_image(facePath[i].c_str(), &width, &height, 0, SOIL_LOAD_RGB);
		if (imageData) {
			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, imageData);
		} else {
			std::cout << "Cubemap texture failed to load at path: " << facePath[i].c_str() << std::endl;
		}
		SOIL_free_image_data(imageData);
	}

	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

	glBindTexture(GL_TEXTURE_CUBE_MAP, 0);

}

void SkyBox::InitShader() {
	skyboxShader = new BaseShader("Skybox/skybox");

	projMatrixID = skyboxShader->GetUniformLocation("P");
	viewMatrixID = skyboxShader->GetUniformLocation("V");
	cubemapID = skyboxShader->GetUniformLocation("skyboxCubeMap");
	glUniform1i(cubemapID, 0);

	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), &skyboxVertices, GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (GLvoid*)0);

	skyboxShader->Use();
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_CUBE_MAP, cubeMapId);
}


void SkyBox::Initialize() {
	InitCubeMap();
	InitShader();
}

void SkyBox::Render(Camera* cam_) {
	skyboxShader->Use();
	glm::mat4 viewMat = glm::mat4(glm::mat3(cam_->Vmatrix()));//remove translation, remain rotation

	glUniformMatrix4fv(viewMatrixID, 1, GL_FALSE, glm::value_ptr(viewMat));
	glUniformMatrix4fv(projMatrixID, 1, GL_FALSE, glm::value_ptr(cam_->Pmatrix()));

	glBindVertexArray(VAO);

	glDrawArrays(GL_TRIANGLES, 0, 36);//18(vts per face) * 6(directions) / 3(vts per triangle) -> 36(triangles)	
}