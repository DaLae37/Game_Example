#include "Coin.h"
#include "Global.h"

Coin::Coin()
{
	io = new IObject();
	sprite = new Sprite("coin-yellow.png");
	AddChild(sprite);
	SetCenter(sprite->width, sprite->height, sprite);
	RunTime = 1.f;
	RunTimer = 0;
}

Coin::~Coin()
{

}

void Coin::Update(float dTime)
{
	IObject::Update(dTime);
	pos.x -= io->runspeed *dTime;
	if (RunTimer > RunTime)
	{
		RunTimer = 0;
		io->runspeed += 10;
	}
	RunTimer += dTime;
}

void Coin::Render()
{
	IObject::Render();
	sprite->Render();
}