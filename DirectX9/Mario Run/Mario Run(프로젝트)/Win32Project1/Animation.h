#pragma once
#include "iobject.h"
#include "Sprite.h"
#include <vector>

class Animation :
	public IObject
{
public:
	Animation(int fps);
	~Animation(void);

	void Render();
	void Update(float dTime);
	int height;
	int width;
	int maxFrame;
	int currentFrame;
	int fps;

	D3DCOLOR color;
	float timeChecker;

	void AddFrame(char*path);

	vector<Sprite*> spriteVector;
};

