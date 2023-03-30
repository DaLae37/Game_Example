#pragma once
#include "IObject.h"
#include "Animation.h"
class Player :
	public IObject
{
public:
	Player();
	~Player();
	Animation* ani;
	Sprite* sprite;
	IObject *io;
	void Render();
	float movespeed;
	float anispeed;
	float gravity;
	bool isJump;
	bool doubleJump;
	void Update(float dTime);
	float RunTime;
	float RunTimer;
};

