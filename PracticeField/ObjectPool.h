#pragma once

#include <vector>

class IUpdatable;
class GameObject;
class BaseLight;
class Renderer;
class Camera;

class ObjectPool
{
private:
	

//	Shader* shadowMapShader;

public:
	ObjectPool();	

	

	std::vector<BaseLight*>* GetLights();
	std::vector<Renderer*>* GetRenderers();


	~ObjectPool();
};

