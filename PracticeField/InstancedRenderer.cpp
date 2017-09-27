#include "InstancedRenderer.h"
#include "GameObject.h"
#include "Shader.h"
#include "FileManager.h"
#include "MeshModel.h"



InstancedRenderer::InstancedRenderer()
{
}


InstancedRenderer::~InstancedRenderer()
{
}

void InstancedRenderer::AddObject(GameObject* go_){
	attachedTransforms.push_back(go_->transform);
}

void InstancedRenderer::AddTransform(Transform * tr_){
	attachedTransforms.push_back(tr_);
}

void InstancedRenderer::SetDefaultShader(){
	shader = FileManager::LoadShader(DefaultVS_Ist, DefaultFS);

	id_matrice.mvp = shader->GetUniformLocation("MVP");
	id_matrice.view = shader->GetUniformLocation("V");

	id_diffuse.count = shader->GetUniformLocation("texCountDiff");
	id_diffuse.id = shader->GetUniformLocation("texture_diffuse");
	id_specular.count = shader->GetUniformLocation("texCountSpec");
	id_specular.id = shader->GetUniformLocation("texture_specular");

	id_dLight.direction = shader->GetUniformLocation("directionalLight0.direction");
	id_dLight.color = shader->GetUniformLocation("directionalLight0.color");
	id_dLight.power = shader->GetUniformLocation("directionalLight0.power");
	id_dLight.lightSpaceMatrix = shader->GetUniformLocation("directionalLight0.lightSpaceMatrix");
	id_dLight.shadowMap = shader->GetUniformLocation("directionalLight0.shadowMap");

	id_pLight.position = shader->GetUniformLocation("pointLight0.position_worldspace");
	id_pLight.color = shader->GetUniformLocation("pointLight0.color");
	id_pLight.power = shader->GetUniformLocation("pointLight0.power");

	shader->Use();
	glUniform1i(id_dLight.shadowMap, TEXTURE_IDX_SHADOWMAP);
	glUniform1i(id_diffuse.id, 0);
	glUniform1i(id_specular.id, 1);
}

void InstancedRenderer::Render(Camera * cam, std::vector<BaseLight*> lights){
	//glBindBuffer(GL_ARRAY_BUFFER, m_Buffers[WVP_MAT_VB]);
	//glBufferData(GL_ARRAY_BUFFER, sizeof(Matrix4f) * NumInstances, WVPMats, GL_DYNAMIC_DRAW);

}

void InstancedRenderer::SetMeshModel(MeshModel * meshModel_){
	meshModel = meshModel_;

	if (meshModel->isSetup == false) {
		int meshCount = meshModel->meshes->size();
		for (int loop = 0; loop < meshCount; loop++) {
			Mesh* currentMesh = meshModel->meshes->at(loop);

			glGenVertexArrays(1, &currentMesh->VAO);
			glBindVertexArray(currentMesh->VAO);

			glGenBuffers(1, &currentMesh->VBO);
			glBindBuffer(GL_ARRAY_BUFFER, currentMesh->VBO);
			if (isStatic) {
				glBufferData(GL_ARRAY_BUFFER, currentMesh->vertices.size() * sizeof(Vertex), &currentMesh->vertices[0], GL_STATIC_DRAW);
			} else {
				glBufferData(GL_ARRAY_BUFFER, currentMesh->vertices.size() * sizeof(Vertex), &currentMesh->vertices[0], GL_DYNAMIC_DRAW);
			}

			// Vertex Positions
			glEnableVertexAttribArray(AttrLoc_Position);
			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)0);
			// Vertex Normals
			glEnableVertexAttribArray(AttrLoc_Normal);
			glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, Normal));
			// Vertex Texture Coords
			glEnableVertexAttribArray(AttrLoc_TexCoord);
			glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, TexCoords));

			InitInstanced(currentMesh);

			glGenBuffers(1, &currentMesh->EBO);
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, currentMesh->EBO);
			if (isStatic) {
				glBufferData(GL_ELEMENT_ARRAY_BUFFER, currentMesh->triangles.size() * sizeof(Triangle),
					&currentMesh->triangles[0], GL_STATIC_DRAW);
			}
			else {
				glBufferData(GL_ELEMENT_ARRAY_BUFFER, currentMesh->triangles.size() * sizeof(Triangle),
					&currentMesh->triangles[0], GL_DYNAMIC_DRAW);
			}

			glBindVertexArray(0);
			glBindBuffer(GL_ARRAY_BUFFER, 0);
		}

		meshModel->isSetup = true;
	}
}

void InstancedRenderer::InitInstanced(Mesh* currentMesh_) {
	glGenBuffers(1, &currentMesh_->instanceVBO);
	glBindBuffer(GL_ARRAY_BUFFER, currentMesh_->instanceVBO);

	if (isStatic) {
		int transformCount = attachedTransforms.size();
		matriceModel = new glm::mat4[transformCount];
		for (int loop = 0; loop < transformCount; loop++) {
			matriceModel[loop] = ComputeModelMatrix(attachedTransforms[loop]);
		}

		glBufferData(GL_ARRAY_BUFFER, attachedTransforms.size() * sizeof(glm::mat4), &matriceModel[0], GL_STATIC_DRAW);
	}

	glEnableVertexAttribArray(AttrLoc_IstMatrix);
	glBindBuffer(GL_ARRAY_BUFFER, currentMesh_->instanceVBO);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glVertexAttribDivisor(AttrLoc_IstMatrix, 1);
}