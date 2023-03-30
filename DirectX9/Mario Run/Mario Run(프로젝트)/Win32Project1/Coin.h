#pragma once
#include "IObject.h"
#include "Sprite.h"
class Coin :
	public IObject
{
public:
	Coin();
	~Coin();
	void Update(float dTime);
	void Render();
	Sprite * sprite;
	IObject *io;
	float RunTime;
	float RunTimer;
};

