#pragma once
#include "Common.h"

HRESULT InitWindow(_In_ HINSTANCE hInstance, _In_ INT nCmdShow);
HRESULT InitDevice();
void CleanupDevice();
void Render();

struct SimpleVertex
{
	XMFLOAT3 Pos;
	XMFLOAT4 Color;
};

struct ConstantBuffer {
	XMMATRIX World;
	XMMATRIX View;
	XMMATRIX Projection;
};

HRESULT CompileShaderFromFile
(
	_In_ PCWSTR pszFileName, // FileName
	_In_ PCSTR pszEntryPoint, // EntryPoint
	_In_ PCSTR pszShaderModel, // Shader target
	_Outptr_ ID3DBlob** ppBlobOut // ID3DBlob out
);