#include "Texture/Material.h"

Material::Material(_In_ std::wstring szName)
	: pNormal()
	, pDiffuse()
	, pSpecular()
	, m_szName(m_szName)
{
}

HRESULT Material::Initailize(ID3D11Device* pDevice, ID3D11DeviceContext* pImmediateContext)
{
	HRESULT hr = S_OK;

	if (pDiffuse) {
		hr = pDiffuse->Initialize(pDevice, pImmediateContext);
		if (FAILED(hr)) {
			return hr;
		}
	}
	if (pSpecular) {
		hr = pSpecular->Initialize(pDevice, pImmediateContext);
		if (FAILED(hr)) {
			return hr;
		}
	}
	if (pNormal) {
		hr = pNormal->Initialize(pDevice, pImmediateContext);
		if (FAILED(hr)) {
			return hr;
		}
	}

	return hr;
}

std::wstring Material::GetName() const
{
	return m_szName;
}
