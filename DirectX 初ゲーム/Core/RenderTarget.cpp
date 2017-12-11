#include "RenderTarget.h"
#include "Common.h"

RenderTarget RenderTarget::back_buffer;

RenderTarget::RenderTarget(int width, int height)
{
	this->width = width;
	this->height = height;
	this->pTexture = nullptr;
	this->pSurface = nullptr;
	this->pDepthSurface = nullptr;
}

RenderTarget::~RenderTarget(void)
{
	SafeRelease(pTexture);
	SafeRelease(pSurface);
	SafeRelease(pDepthSurface);
}

HRESULT RenderTarget::Create(void)
{
	auto pDevice = Direct3D::GetDevice();

	// テクスチャの作成
	if (FAILED(pDevice->CreateTexture(
		width, height,
		1,
		D3DUSAGE_RENDERTARGET,
		D3DFMT_A8R8G8B8,
		D3DPOOL_DEFAULT,
		&pTexture,
		NULL
	)))
	{
		return E_FAIL;
	}

	// テクスチャサーフェースの取得
	if (FAILED(pTexture->GetSurfaceLevel(0, &pSurface)))
		return E_FAIL;

	// 深度バッファの作成
	if (FAILED(pDevice->CreateDepthStencilSurface(
		width, height,
		D3DFMT_D16,
		D3DMULTISAMPLE_NONE,
		0,
		TRUE,
		&pDepthSurface,
		NULL
	)))
	{
		return E_FAIL;
	}

	return S_OK;
}

RenderTarget* RenderTarget::BackBuffer(void)
{
	if (back_buffer.pSurface == nullptr)
	{
		auto pDevice = Direct3D::GetDevice();

		LPDIRECT3DSURFACE9 pSurface;
		LPDIRECT3DSURFACE9 pDepthSurface;

		if (FAILED(pDevice->GetRenderTarget(0, &pSurface)))
			return nullptr;

		if (FAILED(pDevice->GetDepthStencilSurface(&pDepthSurface)))
			return nullptr;

		back_buffer.pSurface = pSurface;
		back_buffer.pDepthSurface = pDepthSurface;

	}
	
	return &back_buffer;
}
