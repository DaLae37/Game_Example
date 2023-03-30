#include "Scene.h"


Scene::Scene(void)
{
}


Scene::~Scene(void)
{
	for(auto& object:objectList) {
		delete object;
	}

}

void Scene::Render() {
	

}

void Scene::Update(float dTime) {
	for(auto&object: objectList)   {
		object->Update(dTime);
	}

}
void Scene::AddObject(IObject*object) {
	objectList.push_back(object);
	object->parent= nullptr;
}

void Scene::RemoveObject(IObject*object) {
	objectList.remove(object);
	object->parent= nullptr;
}