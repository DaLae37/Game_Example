#pragma once
#include "Scene.h"
#include "Sprite.h"
#include "IObject.h"
class HowScene :
	public Scene
{
public:
	HowScene();
	~HowScene();
	Sprite *howscene;
	Sprite *howout;
	void Update(float dTime);
	void Render();
};

