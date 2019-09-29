#pragma once


#include <string>
#include <gl\glew.h>
#include <glm\glm.hpp>
#include <vector>

#include <Bases/EngineResource.h>

#define LogLength 512

#define dirPathShader "../Shaders/"
#define TEXTURE_IDX_SHADOWMAP 10

using namespace std;

struct ID_matrice {
	GLuint mvp = -1;
	GLuint view = -1;
	GLuint model = -1;
	GLuint vp = -1;
};

struct ID_dLight {
	GLuint direction;
	GLuint color;
	GLuint power;
	GLuint lightSpaceMatrix;
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
//상속 받은 뒤 생성자에 매칭되는 shader의 filePath를 기입해야 함
class BaseShader : public EngineResource {
private:
	

protected:
	GLuint shaderID;
	std::string ReadCodeFromFile(std::string path_);
	void CompileCode(int shaderId_, const GLchar* code_);
	GLuint CreateShader(GLchar* path_, int shaderType_);
	void CreateProgram(GLuint shader0, GLuint shader1, GLuint shader2);
	void CreateProgram(GLuint shader0, GLuint shader1);

	void LoadProgram(string vertexPath_, string geometryPath_, string fragmentPath_);	

	string filePathVertex = "";
	string filePathGeometry = "";
	string filePathFragment = "";

public:
	BaseShader();
	BaseShader(std::string filePathVertex_, std::string filePathFragment_);
	BaseShader(std::string filePathVertex_, std::string filePathGeometry_, std::string filePathFragment_);
	virtual ~BaseShader();
	string GetDirectoryVertex();
	string GetDirectoryGeometry();
	string GetDirectoryFragment();		

	void Use();
		
	GLuint GetUniformLocation(const GLchar* var_name);
	void SetMat4(string var_name, glm::mat4 mat4_);
	void SetVec3(string var_name, glm::vec3 vec3_);
	void SetInt(string var_name, int val_);

	static BaseShader* GetShader(std::string filePathVertex, std::string filePathGeometry, std::string filePathFragment);


	virtual void Initialize() {}
	//virtual void SetUniforms(RenderData* renderData_, glm::mat4 modelMat_, glm::mat4 mvpMat_) {}
	virtual void SetUniforms(Camera* camera_, MeshRenderer* renderer_, std::vector<BaseLight*>* lights_) {}
	virtual void ApplyTexture(std::vector<Texture*> textures_) {}
	virtual void OnEndUse();
};

