#include "Renderer_Deferred.h"


#include "../Mesh/MeshModel.h"
#include "RenderData.h"

//#include <gl/glew.h>
#include <glm/gtc/type_ptr.hpp>

Renderer_Deferred::Renderer_Deferred() {
}


Renderer_Deferred::~Renderer_Deferred() {
}

void Renderer_Deferred::Render(RenderData * renderData_) {		
	/*for (GLuint loop = 0; loop < meshModel->meshes->size(); loop++) {
		Mesh* processingMesh = meshModel->meshes->at(loop);

		shader->ApplyTexture(processingMesh->textures);

		glBindVertexArray(processingMesh->VAO);

		glDrawElements(
			GL_TRIANGLES,
			processingMesh->GetIdxCount(),
			GL_UNSIGNED_INT,
			NULL
		);
	}*/

	glBindVertexArray(0);
	glBindTexture(GL_TEXTURE_2D, 0);
}
