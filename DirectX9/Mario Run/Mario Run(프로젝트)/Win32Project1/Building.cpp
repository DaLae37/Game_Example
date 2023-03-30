#include "Building.h"
#include "Global.h"
Building::Building()
{
	io = new IObject();
	sprite = new Sprite("Block.png");
	AddChild(sprite);
	SetCenter(sprite->width, sprite->height, sprite);
}

Building::~Building()
{
}

void Building::Render()
{
	IObject::Render();
	sprite->Render();
}

void Building::Update(float dTime)
{
	IObject::Update(dTime);
	pos.x -= io->runspeed * dTime;
}
