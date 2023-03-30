#pragma once
#include "IObject.h"
#include "Scene.h"
#include "Sprite.h"
class MainScene :
	public Scene
{
public:
	MainScene();
	~MainScene();
	Sprite* startButton;
	Sprite* exitButton;
	Sprite* mainBack;
	Sprite* name;
	Sprite* how;
	void Render();
	void Update(float dTime);
};

