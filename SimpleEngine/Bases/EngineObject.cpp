#include "EngineObject.h"

#include "BaseScript.h"
#include "../Render/DefaultRenderer.h"
#include "../FilePooler.h"
#include "../Scenes/Scene.h"
#include <typeinfo>

EngineObject::EngineObject(){
	transform = new Transform();
	transform->gameObject = this;
	Scene::RegisterObject(this);	

	if (renderer != NULL)
		renderer->Initialize();

	for (int loop = 0; loop < components.size(); loop++) {
		components[loop]->Initialize();
	}
}

EngineObject::EngineObject(std::string name_){
	name = name_;
	transform = new Transform();
	transform->gameObject = this;
	Scene::RegisterObject(this);

	if (renderer != NULL)
		renderer->Initialize();

	for (int loop = 0; loop < components.size(); loop++) {
		components[loop]->Initialize();
	}
}

void EngineObject::SetId(unsigned int id_){
	objectId = id_;
}

unsigned int EngineObject::GetId(){
	return objectId;
}

void EngineObject::SetRenderer(BaseRenderer * renderer_){
	renderer_->SetTransform(transform);

	Scene::GetCurrent()->AddRenderer(renderer_);
	renderer = renderer_;
}

BaseRenderer * EngineObject::GetRenderer(){
	return renderer;
}

EngineObject::~EngineObject(){	
	free(renderer);
	free(transform);
	for (int loop = 0; loop < components.size(); loop++) {
		free(components[loop]);
	}
}