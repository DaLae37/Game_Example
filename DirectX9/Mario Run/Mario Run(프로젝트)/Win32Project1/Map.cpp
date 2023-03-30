#include "Map.h"
#include "Global.h"

Map::Map()
{	
	io = new IObject();
	sprite = new Sprite("LoopMap.png");
	AddChild(sprite);
	RunTime = 1.f;
	RunTimer = 0.f;
}

Map::~Map()
{

}

void Map::Render()
{
	IObject::Render();
	sprite->Render();
}

void Map::Update(float dTime)
{
	IObject::Update(dTime);
	pos.x -= io->runspeed*dTime;
	if (RunTimer > RunTime)
	{
		RunTimer = 0;
		io->runspeed += 10;
	}
	RunTimer += dTime;
}