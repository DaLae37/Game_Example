#include "Number.h"
#include "Global.h"

Number::Number()
{
	num[1] = new Sprite("number-1.png");
	AddChild(num[1]);
	num[2] = new Sprite("number-2.png");
	AddChild(num[2]);
	num[3] = new Sprite("number-3.png");
	AddChild(num[3]);
	num[4] = new Sprite("number-4.png");
	AddChild(num[4]);
	num[5] = new Sprite("number-5.png");
	AddChild(num[5]);
	num[6] = new Sprite("number-6.png");
	AddChild(num[6]);
	num[7] = new Sprite("number-7.png");
	AddChild(num[7]);
	num[8] = new Sprite("number-8.png");
	AddChild(num[8]);
	num[9] = new Sprite("number-9.png");
	AddChild(num[9]);
	num[0] = new Sprite("number-0.png");
	AddChild(num[0]);
}

Number::~Number()
{
}

void Number::Update(float dTime)
{
	IObject::Update(dTime);
}

void Number::Render()
{
	IObject::Render();
}

void Number::Set()
{
	for(int i=0; i<10; i++)
	SetCenter(num[i]->width, num[i]->height, num[i]);
}