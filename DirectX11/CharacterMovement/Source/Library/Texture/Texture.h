#pragma once

#include "Common.h"
#include "WICTextureLoader.h"

class Texture {
protected :
	std::filesystem::path m_textureFilePath;
	ComPtr<ID3D11ShaderResourceView> m_textureRV;
	ComPtr<ID3D11SamplerState> m_samplerLinear;
public :
	Texture(const std::filesystem::path path);
	~Texture();
	HRESULT Initialize(_In_ ID3D11Device* pDevice,
		_In_ ID3D11DeviceContext* pImmediateContext);
	ComPtr<ID3D11ShaderResourceView>& GetTextureResourceView();
	ComPtr<ID3D11SamplerState>& GetSamplerState();
};