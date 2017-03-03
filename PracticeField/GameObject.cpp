#include "GameObject.h"



GameObject::GameObject(){
	renderer = new Renderer(&transform);
}

void GameObject::Render(Camera* cam){
	renderer->Render(cam);
}

void GameObject::Update(){			
	transform.position.x += Time::deltaTime * 0.001f;
}


GameObject::~GameObject(){	
}
