#include "ObjectPool.h"



ObjectPool::ObjectPool(){
}

std::vector<GameObject*> ObjectPool::GetObject(){
	return objects;
}

void ObjectPool::AddObejct(GameObject * obj){
	objects.push_back(obj);
}

void ObjectPool::UpdateObjects(){
	for (int loop = 0; loop < objects.size(); loop++) {
		objects.at(loop)->Update();
	}
}

void ObjectPool::RenderObjects(Camera* cam){
	for (int loop = 0; loop < objects.size(); loop++) {
		objects.at(loop)->Render(cam);
	}
}


ObjectPool::~ObjectPool(){
}
