//=============================================================================
//
// ファイル名：graphics.h
// 作者：GP11A143 38 楊子毅
//
//=============================================================================
#pragma once

#include <d3dx9.h>
#include <vector>

class ILostAndReset
{
public:
	ILostAndReset(void);
	~ILostAndReset(void);
	virtual HRESULT OnLostDevice(void) = 0;
	virtual HRESULT OnResetDevice(void) = 0;
};


class Direct3D
{
	friend class ILostAndReset;
public:
	static HRESULT Init(HWND hWnd, bool bWindowMode);
	static void Uninit(void);
	static LPDIRECT3DDEVICE9 GetDevice(void);
	static bool ResetDevice(void);
	static LPD3DXFONT GetFont(void);
	static bool SetWindowMode(bool windowMode);
#ifdef _DEBUG
	static void ShowErrorMesg(const HRESULT &hr);
#endif

private:
	static LPDIRECT3D9				s_pD3D;		// Direct3Dオブジェクト
	static LPDIRECT3DDEVICE9		s_pDevice;	// Deviceオブジェクト(描画に必要)
	static LPD3DXFONT				s_pFont;	// フォント
	static D3DPRESENT_PARAMETERS	s_d3dpp;	// デバイスのプレゼンテーションパラメータ
	static HWND						s_hWnd;		// ウィンドウハンドル
	static std::vector<ILostAndReset*> s_reset_list;
};


