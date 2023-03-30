#pragma once
#include<d3dx9.h>
#include<map>


using namespace std;

class TextureManager
{
public:
	TextureManager(void);
	~TextureManager(void);

	map<char*, LPDIRECT3DTEXTURE9> textureMap;
	LPDIRECT3DTEXTURE9 LoadTextureFromFile(char* path);

	
};

