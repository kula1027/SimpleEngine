#include "ObjectPool.h"



ObjectPool::ObjectPool(){
}

void ObjectPool::AddGameObject(GameObject * obj){
	gameObjects.push_back(obj);
	updatables.push_back(obj);
}

void ObjectPool::AddUpdatable(IUpdatable * obj){
	updatables.push_back(obj);
}

void ObjectPool::AddLight(Light * objLight){
	lights.push_back(objLight);
	updatables.push_back(objLight);
}

void ObjectPool::UpdateObjects(){
	for (int loop = 0; loop < updatables.size(); loop++) {
		updatables.at(loop)->Update();
	}
}

void ObjectPool::RenderObjects(Camera* cam){
	for (int loop = 0; loop < gameObjects.size(); loop++) {
		gameObjects.at(loop)->Render(cam, lights);
	}
}


ObjectPool::~ObjectPool(){
}
