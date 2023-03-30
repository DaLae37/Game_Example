#pragma once
#include "IObject.h"
#include "Sprite.h"
#include "Player.h"
class Map :
	public IObject
{
public:
	Map();
	~Map();
	IObject *io;
	void Render();
	void Update(float dTime);
	Player *player;
	Sprite *sprite;
	float RunTime;
	float RunTimer;
};

