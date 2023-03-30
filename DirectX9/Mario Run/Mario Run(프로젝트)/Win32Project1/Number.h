#pragma once
#include "IObject.h"
#include "Sprite.h"
class Number :
	public IObject
{
public:
	Number();
	~Number();
	void Update(float dTime);
	void Render();
	Sprite* num[10];
	void Set();
};

