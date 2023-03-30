#include"Global.h"

LPDIRECT3D9 g_pd3d;
D3DPRESENT_PARAMETERS d3dpp;
LPDIRECT3DDEVICE9 g_pd3dDevice;
LPD3DXSPRITE g_pd3dSprite;
HWND g_hWnd;
SceneManager*sceneManager;
TextureManager* textureManager;

bool lkey[256],rkey[256];

void UpdateKeyState() {
	for(int i=0;i<256;i++) {
		lkey[i]=rkey[i];
		rkey[i]=GetAsyncKeyState(i)& 0x8000;
	}
}

int GetMyKeyState(int vk) {
	if(lkey[vk] && rkey[vk]) {
		return 2;
	}
	if(!lkey[vk] && rkey[vk]) {
		return 1;
	}
	if(lkey[vk] && !rkey[vk]) {
		return -1;
	}
		return 0;
}

bool IsCollisionRect(D3DXVECTOR2 p1, D3DXVECTOR2 p2, RECT r1, RECT r2)
{
	RECT r;
	r1.left += p1.x;
	r1.right += p1.x;
	r1.top += p1.y;
	r1.bottom += p1.y;

	r2.left += p2.x;
	r2.right += p2.x;
	r2.top += p2.y;
	r2.bottom += p2.y;

	return IntersectRect(&r, &r1, &r2);
}

D3DXVECTOR2 GetMousePos()
{
	POINT pt;
	GetCursorPos(&pt);
	ScreenToClient(g_hWnd, &pt);
	return Vec2(pt.x, pt.y);
}
bool IsPointInRect(D3DXVECTOR2 p, RECT r, D3DXVECTOR2 pt)
{
	r.left += p.x;
	r.right += p.x;
	r.top += p.y;
	r.bottom += p.y;

	return (pt.x > r.left && pt.x<r.right && pt.y>r.top&&pt.y < r.bottom);
}
