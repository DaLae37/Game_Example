#include<Windows.h>
#include<d3dx9.h>
#include<iostream>

using namespace std;

#define NAME TEXT("API")

HWND FloatWindow(HINSTANCE hlnst, int nCmdShow);

int DoMainLoop();
void InitWindow(HINSTANCE hlnst);

LRESULT CALLBACK WndProc(HWND,UINT,WPARAM,LPARAM);

void lnitD3D(HWND hWnd);
void ReleaseD3D();

void lnitDeltaTime();
float getDeltaTime();

void lnitManager();
void DeleteManager();


