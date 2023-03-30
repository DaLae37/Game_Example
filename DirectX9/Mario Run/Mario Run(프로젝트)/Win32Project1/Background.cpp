#include "Background.h"
#include "Global.h"

Background::Background()
{
	io = new IObject();
	sprite = new Sprite("Background.png");
	AddChild(sprite);
	movespeed = io->runspeed;
	RunTime = 0;
	RunTimer = 1.f;
}


Background::~Background()
{
}

void Background::Render()
{
	IObject::Render();
	sprite->Render();
}

void Background::Update(float dTime)
{
	IObject::Update(dTime);
	pos.x -= io->runspeed * dTime;
}
