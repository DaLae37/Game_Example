#pragma once
#include "IObject.h"
#include "Sprite.h"
class Background :
	public IObject
{
public:
	IObject *io;
	Background();
	~Background();
	Sprite *sprite;
	void Update(float dTime);
	void Render();
	float movespeed;
	float RunTimer;
	float RunTime;
};

