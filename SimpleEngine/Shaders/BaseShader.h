#pragma once


#include <string>
#include <gl\glew.h>
#include <glm\glm.hpp>
#include <vector>

#include <Bases/EngineResource.h>

#define LogLength 512

#define TEXTURE_IDX_SHADOWMAP 10

using namespace std;

struct ID_matrice {
	GLuint mvp = -1;	
	GLuint model = -1;	
};

struct ID_dLight {
	GLuint direction;
	GLuint color;
	GLuint power;
	GLuint lightVP;
	GLuint shadowMap;
};

struct ID_pLight {
	GLuint position;
	GLuint color;
	GLuint power;
};

struct ID_diffuseTexture {
	GLuint count;
	GLuint id;
};

struct ID_specularTexture {
	GLuint count;
	GLuint id;
};

class Camera;
class BaseLight;
class MeshRenderer;
class RenderData;
class Mesh;
class Texture;


//shader파일들과 매칭되는 클라스
class BaseShader : public EngineResource {
private:	
	static int currentUsingShader;

protected:
	GLuint shaderID;
	string filePath = "NotInitialized";

	std::string ReadCodeFromFile(std::string path_);
	void CompileCode(int shaderId_, const GLchar* code_);
	GLuint CreateShader(GLchar* path_, int shaderType_);
	void CreateProgram(GLuint shader0, GLuint shader1, GLuint shader2);
	void CreateProgram(GLuint shader0, GLuint shader1);
	void LoadProgram(string filePath_);	
	
	ID_matrice id_matrice;
public:
	BaseShader();
	BaseShader(std::string filePath_);
	virtual ~BaseShader();
		
	string GetFilePath();

	void Use();
		
	GLuint GetUniformLocation(const GLchar* var_name);
	GLuint GetUniformBlockIndex(const GLchar* var_name);
	void SetMat4(string var_name, glm::mat4 mat4_);
	void SetVec3(string var_name, glm::vec3 vec3_);
	void SetInt(string var_name, int val_);
	void SetMat_M(glm::mat4 matM);
	void SetMat_MVP(glm::mat4 matMVP);

	static BaseShader* GetShader(std::string filePathVertex, std::string filePathGeometry, std::string filePathFragment);

	//virtual void SetUniforms(RenderData* renderData_, glm::mat4 modelMat_, glm::mat4 mvpMat_) {}
	virtual void SetUniforms(MeshRenderer* renderer_) {}
	virtual void ApplyTexture(std::vector<Texture*> textures_) {}


	void BindLightUBO();
	void BindCameraUBO();
};

