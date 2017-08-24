#include "ObjectPool.h"

#include "Camera.h"
#include "GameObject.h"
#include "IUpdatable.h"
#include "Renderer.h"
#include "Lights.h"


ObjectPool::ObjectPool(){
}

void ObjectPool::AddGameObject(GameObject * obj){
	gameObjects.push_back(obj);
}

void ObjectPool::AddUpdatable(IUpdatable * upd){
	updatables.push_back(upd);
}

void ObjectPool::AddRenderer(Renderer * rdr){
	renderers.push_back(rdr);
}

void ObjectPool::AddLight(BaseLight * objLight){
	lights.push_back(objLight);
}

void ObjectPool::UpdateObjects(){
	for (int loop = 0; loop < updatables.size(); loop++) {
		updatables[loop]->Update();
	}
}

void ObjectPool::RenderObjects(Camera* cam){
	int rSize = renderers.size();
	for (int loop = 0; loop < rSize; loop++) {
		renderers[loop]->Render(cam, lights);
	}
}


ObjectPool::~ObjectPool(){
}
