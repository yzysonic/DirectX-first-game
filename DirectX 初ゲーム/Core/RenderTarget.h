#pragma once
#include "Direct3D.h"
#include "Texture.h"
#include "Common.h"
#include <vector>

class RenderTarget : public Texture, public ILostAndReset
{
public:
	static RenderTarget* BackBuffer(void);

private:
	static RenderTarget back_buffer;
	static std::vector<RenderTarget*> render_target_list;

public:
	LPDIRECT3DSURFACE9 pSurface;
	LPDIRECT3DSURFACE9 pDepthSurface;

	HRESULT OnLostDevice(void) override;
	HRESULT OnResetDevice(void) override;

	RenderTarget(int width = SystemParameters::ResolutionX, int height = SystemParameters::ResolutionY, bool create = true);
	RenderTarget(bool create);
	~RenderTarget(void);

private:
	HRESULT Create(void);
	int index;
};