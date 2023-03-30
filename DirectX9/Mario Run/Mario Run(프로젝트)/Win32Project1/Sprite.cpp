#include "Sprite.h"
#include "Global.h"

Sprite::Sprite(char *path)
{
	texture= textureManager->LoadTextureFromFile(path);
	D3DSURFACE_DESC desc;
	texture->GetLevelDesc(0,&desc);

	width= desc.Width;
	height= desc.Height;

	visibleRect.left=0;
	visibleRect.top= 0;
	visibleRect.right= width;
	visibleRect.bottom= height;

	rect = visibleRect;

	color= D3DCOLOR_ARGB(255,255,255,255);
}


Sprite::~Sprite(void)
{
}

void Sprite::Render() {
	IObject::Render();
	g_pd3dSprite->SetTransform(&mat);
	g_pd3dSprite->Draw(texture,&visibleRect,NULL,NULL,color);

}