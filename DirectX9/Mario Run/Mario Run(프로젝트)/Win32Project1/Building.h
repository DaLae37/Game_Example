#pragma once
#include "IObject.h"
#include "Sprite.h"
class Building :
	public IObject
{
public:
	Building();
	~Building();
	IObject *io;
	void Update(float dTime);
	void Render();
	Sprite *sprite;
};

