#pragma once

#ifndef UNICODE
#define UNICODE
#endif

#include <windows.h>

#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "d3dcompiler.lib")
#pragma comment(lib, "dxguid.lib")

#include <d3d11_4.h>
#include <DDSTextureLoader11.h>
#include <d3dcompiler.h>
#include <directxcolors.h>

#include <iostream>
#include <string>

using namespace DirectX;

// ASSIMP
#define ASSIMP_LOAD_FLAGS  (aiProcess_Triangulate | aiProcess_GenSmoothNormals | aiProcess_FlipUVs | aiProcess_JoinIdenticalVertices | aiProcess_ConvertToLeftHanded)