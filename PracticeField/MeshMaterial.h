#pragma once
#include <glm\glm.hpp>
#include <vector>

class MeshMaterial{
private:
	std::vector<glm::vec3> vertexData;
	std::vector<glm::vec2> uvData;
	std::vector<glm::vec3> normalData;

public:	
	MeshMaterial();
	~MeshMaterial();

	char* filePath;
	

};

