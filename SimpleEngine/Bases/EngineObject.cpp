#include "EngineObject.h"

#include "BaseScript.h"
#include "../Render/DefaultRenderer.h"
#include "../FilePooler.h"
#include "../Scenes/Scene.h"
#include <typeinfo>

void EngineObject::Initialize() {
	transform = new Transform();
	transform->engineObject = this;
	Scene::RegisterObject(this);

	if (renderer != NULL)
		renderer->Initialize();
}

EngineObject::EngineObject(){
	Initialize();
}

EngineObject::EngineObject(std::string name_){
	name = name_;
	Initialize();
}

void EngineObject::SetId(unsigned int id_){
	objectId = id_;
}

unsigned int EngineObject::GetId(){
	return objectId;
}


BaseComponent * EngineObject::AttachComponent(BaseComponent* baseComponent_) {
	components.push_back(baseComponent_);
	baseComponent_->OnAttachedToObject(this);

	if (BaseScript* script = dynamic_cast<BaseScript*>(baseComponent_)) {
		script->engineObject = this;
		script->transform = this->transform;
		Scene::RegisterScript(script);

		script->OnStart();
	}

	return nullptr;
}

EngineObject::~EngineObject(){	
	free(renderer);
	free(transform);
	for (int loop = 0; loop < components.size(); loop++) {
		free(components[loop]);
	}
}