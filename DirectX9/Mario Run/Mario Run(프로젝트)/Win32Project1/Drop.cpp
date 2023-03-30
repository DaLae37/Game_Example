#include "Drop.h"
#include "Global.h"

Drop::Drop()
{
	io = new IObject();
	sprite = new Sprite("Drop.png");
	SetCenter(sprite->width, sprite->height, sprite);
	AddChild(sprite);
	RunTimer = 0;
	RunTime = 1.f;
}

Drop::~Drop()
{
	
}

void Drop::Render()
{
	IObject::Render();
	sprite->Render();
}
void Drop::Update(float dTime)
{
	IObject::Update(dTime);
	pos.x -= io->runspeed * dTime;
	if (RunTimer > RunTime)
	{
		RunTimer = 0;
		io->runspeed += 10;
	}
	RunTimer += dTime;
}