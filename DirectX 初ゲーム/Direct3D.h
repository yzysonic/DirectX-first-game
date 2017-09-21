//=============================================================================
//
// ファイル名：graphics.h
// 作者：GP11A143 38 楊子毅
//
//=============================================================================
#pragma once

#include <d3dx9.h>

class Direct3D
{
public:
	static HRESULT Init(HWND hWnd, int screenWidth, int screenHeight, bool bWindowMode);
	static void Uninit(void);
	static LPDIRECT3DDEVICE9 GetDevice(void);
	static bool ResetDevice(void);
	static LPD3DXFONT GetFont(void);
	static bool SetWindowMode(bool windowMode);

private:
	static LPDIRECT3D9				s_pD3D;			// Direct3Dオブジェクト
	static LPDIRECT3DDEVICE9		s_pDevice;	// Deviceオブジェクト(描画に必要)
	static LPD3DXFONT				s_pFont;	// フォント
	static D3DPRESENT_PARAMETERS	s_d3dpp;		// デバイスのプレゼンテーションパラメータ
};

