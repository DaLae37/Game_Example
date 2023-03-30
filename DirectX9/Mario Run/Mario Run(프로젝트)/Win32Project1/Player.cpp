#include "Player.h"
#include "Global.h"

Player::Player()
{
	io = new IObject();
	sprite = new Sprite("harphoon.png");
	AddChild(sprite);
	anispeed = IObject::runspeed / 100;
	ani = new Animation(anispeed);
	AddChild(ani);
	isJump = false;
	doubleJump = false;
	ani->AddFrame("player-run.png");
	ani->AddFrame("player-stop.png");
	SetCenter(ani->width, ani->height, ani);
	gravity = 0.f;
	RunTimer = 1.f;
	RunTime = 0;
	movespeed = io->runspeed;
}

Player::~Player()
{
}

void Player::Render()
{
	IObject::Render();
	sprite->Render();
	ani->Render();
}

void Player::Update(float dTime)
{
	gravity += 15.f;
	pos.y += gravity * dTime;
	if (doubleJump)
	{
		pos.y -= 300 * dTime;
	}
	if (isJump)
	{
		pos.y -= 500 * dTime;
		if (GetMyKeyState(VK_SPACE) == 1)
		{
			doubleJump = true;
			cout << "더블점프" << endl;
		}
	}
	if (pos.y > 430)
	{
		pos.y = 430;
		doubleJump = false;
		isJump = false;
	}
	anispeed = io->runspeed *dTime;
	IObject::Update(dTime);
	if (GetMyKeyState(VK_UP) == 1 && pos.y == 430)
	{
		isJump = true;
		gravity = 0;
	}
	if (RunTimer > RunTime)
	{
		RunTimer = 0;
		io->runspeed += 10;
	}
	RunTimer += dTime;
}