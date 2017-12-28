#pragma once
#include "Direct3D.h"
#include "Texture.h"
#include "Common.h"
#include <vector>

class RenderTarget : public Texture
{
public:
	static RenderTarget* BackBuffer(void);
	static void OnLostDevice(void);
	static void OnResetDevice(void);

private:
	static RenderTarget back_buffer;
	static std::vector<RenderTarget*> render_target_list;

public:
	LPDIRECT3DSURFACE9 pSurface;
	LPDIRECT3DSURFACE9 pDepthSurface;
	RenderTarget(int width = SystemParameters::ResolutionX, int height = SystemParameters::ResolutionY, bool create = true);
	~RenderTarget(void);

private:
	HRESULT Create(void);
	int index;
};