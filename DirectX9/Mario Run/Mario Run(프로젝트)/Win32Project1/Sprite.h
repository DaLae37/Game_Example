#pragma once
#include "iobject.h"
class Sprite :
	public IObject
{
public:
	Sprite(char* path);
	~Sprite(void);

	LPDIRECT3DTEXTURE9 texture;
	D3DCOLOR color;
	RECT visibleRect;

	int width;
	int height;


	void Render();

};

