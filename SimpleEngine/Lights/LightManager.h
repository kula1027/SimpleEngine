#pragma once
#include <vector>
#include <map>
#include <glm/vec3.hpp>

class BaseLight;
class DirectionalLight;
class PointLight;
class BaseShader;

class LightManager {

private:	
	static LightManager* instance;
	LightManager();

	unsigned int uboLightData;

	glm::vec3 ambient;

public:			

	static LightManager* Inst();

	void SetAmbient(glm::vec3 ambient_);

	void AddLight(BaseLight* light_);
	std::vector<BaseLight*> lights;
	std::map<unsigned int, PointLight*> pointLights;
	std::map<unsigned int, DirectionalLight*> directionalLights;

};

