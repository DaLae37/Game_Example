#pragma once
#include "IObject.h"
#include "Sprite.h"
class Drop :
	public IObject
{
public:
	IObject *io;
	Drop();
	~Drop();
	void Render();
	void Update(float dTime);
	Sprite *sprite;
	float RunTime;
	float RunTimer;
};

