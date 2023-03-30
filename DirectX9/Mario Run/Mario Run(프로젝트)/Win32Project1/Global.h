#pragma once
#include"SceneManager.h"
#include"TextureManager.h"
#include<d3dx9.h>
#include <iostream>
using namespace std;
#define NAME TEXT("API")
#define Vec2(a,b) D3DXVECTOR2((a),(b))

#define SCREEN_WIDTH  1024
#define SCREEN_HEIGHT 768
#define BG_COLOR D3DCOLOR_ARGB(255,255,255,255)
extern LPDIRECT3D9 g_pd3d;
extern D3DPRESENT_PARAMETERS d3dpp;
extern LPDIRECT3DDEVICE9 g_pd3dDevice;
extern LPD3DXSPRITE g_pd3dSprite;
extern HWND g_hWnd;
extern SceneManager* sceneManager;
extern TextureManager * textureManager;
void UpdateKeyState();
int GetMyKeyState(int vk);

bool IsCollisionRect(D3DXVECTOR2 p1, D3DXVECTOR2 p2, RECT r1, RECT r2);
bool IsPointInRect(D3DXVECTOR2 p, RECT r, D3DXVECTOR2 pt);
D3DXVECTOR2 GetMousePos();