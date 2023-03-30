#pragma once
#include"IObject.h"

class Scene
{
public:
	Scene(void);
	virtual~Scene(void);
	list<IObject*> objectList;

	virtual void Render();
	virtual void Update(float dTime);

	void AddObject(IObject*object);
	void RemoveObject(IObject*object);

};

