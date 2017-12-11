#pragma once
#include "Direct3D.h"
#include "Common.h"

class RenderTarget
{
public:
	static RenderTarget* BackBuffer(void);

public:
	LPDIRECT3DSURFACE9 pSurface;
	LPDIRECT3DSURFACE9 pDepthSurface;
	LPDIRECT3DTEXTURE9 pTexture;

	RenderTarget(int width = SystemParameters::ResolutionX, int height = SystemParameters::ResolutionY);
	~RenderTarget(void);
	HRESULT Create(void);

private:
	int width;
	int height;
	static RenderTarget back_buffer;

};