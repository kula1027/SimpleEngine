#include "GameObject.h"

#include "Renderer.h"
#include "FileManager.h"
#include "Scene.h"

GameObject::GameObject(){
	name = strNoname;	
	transform = new Transform();
	Scene::GetCurrent()->AddGameObject(this);
}

void GameObject::SetRenderer(Renderer * renderer_){
	renderer_->SetTransform(transform);

	Scene::GetCurrent()->AddRenderer(renderer_);
	renderer = renderer_;
}

Renderer * GameObject::GetRenderer(){
	return renderer;
}

void GameObject::SetShader(Shader * shader_) {
	renderer->SetShader(shader_);
}

GameObject::~GameObject(){	
	free(renderer);
	free(transform);
	for (int loop = 0; loop < components.size(); loop++) {
		free(components[loop]);
	}
}