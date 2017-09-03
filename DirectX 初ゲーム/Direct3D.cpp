//=============================================================================
//
// ファイル名：graphics.cpp
// 作者：GP11A143 38 楊子毅
//
//=============================================================================
#include "Direct3D.h"
#include "main.h"
#include "Texture.h"
#include <DxErr.h>

#pragma comment (lib, "d3d9.lib")
#pragma comment (lib, "d3dx9.lib")
#pragma comment (lib, "dxerr.lib")
#pragma comment (lib, "legacy_stdio_definitions.lib")

//*****************************************************************************
// グローバル変数:
//*****************************************************************************
LPDIRECT3D9				g_pD3D = NULL;				// Direct3Dオブジェクト
LPDIRECT3DDEVICE9		g_pD3DDevice = NULL;		// Deviceオブジェクト(描画に必要)
LPD3DXFONT				g_pD3DXFont = NULL;			// フォント
D3DPRESENT_PARAMETERS	g_d3dpp = {};				// デバイスのプレゼンテーションパラメータ

void D3D_SetWindowMode(D3DPRESENT_PARAMETERS *d3dpp, bool windowMode);

//=============================================================================
// グラフィックの初期化処理
//=============================================================================
HRESULT InitDirect3D(HWND hWnd, int screenWidth, int screenHeight, bool bWindowMode)
{
	D3DPRESENT_PARAMETERS d3dpp;
	D3DDISPLAYMODE d3ddm;

	// Direct3Dオブジェクトの作成
	g_pD3D = Direct3DCreate9(D3D_SDK_VERSION);
	if (g_pD3D == NULL)
	{
		return E_FAIL;
	}

	// 現在のディスプレイモードを取得
	if (FAILED(g_pD3D->GetAdapterDisplayMode(D3DADAPTER_DEFAULT, &d3ddm)))
	{
		return E_FAIL;
	}

	// デバイスのプレゼンテーションパラメータの設定
	ZeroMemory(&d3dpp, sizeof(d3dpp));					// ワークをゼロクリア
	d3dpp.BackBufferCount = 1;							// バックバッファの数
	d3dpp.BackBufferWidth = screenWidth;				// ゲーム画面サイズ(幅)
	d3dpp.BackBufferHeight = screenHeight;				// ゲーム画面サイズ(高さ)
	d3dpp.BackBufferFormat = D3DFMT_UNKNOWN;			// バックバッファのフォーマットは現在設定されているものを使う
	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;			// 映像信号に同期してフリップする
	d3dpp.hDeviceWindow = hWnd;							// 使用するウィンド
	d3dpp.EnableAutoDepthStencil = TRUE;				// デプスバッファ（Ｚバッファ）とステンシルバッファを作成
	d3dpp.AutoDepthStencilFormat = D3DFMT_D16;			// デプスバッファとして16bitを使う
	d3dpp.BackBufferFormat = d3ddm.Format;				// カラーモードの指定

	D3D_SetWindowMode(&d3dpp, bWindowMode);

	// デバイスの生成
	// ディスプレイアダプタを表すためのデバイスを作成
	// 描画と頂点処理をハードウェアで行なう
	if (FAILED(g_pD3D->CreateDevice(D3DADAPTER_DEFAULT,							// ディスプレイアダプタ
		D3DDEVTYPE_HAL,								// ディスプレイタイプ
		hWnd,										// フォーカスするウインドウへのハンドル
		D3DCREATE_HARDWARE_VERTEXPROCESSING,		// デバイス作成制御の組み合わせ
		&d3dpp,										// デバイスのプレゼンテーションパラメータ
		&g_pD3DDevice)))							// デバイスインターフェースへのポインタ
	{
		// 上記の設定が失敗したら
		// 描画をハードウェアで行い、頂点処理はCPUで行なう
		if (FAILED(g_pD3D->CreateDevice(D3DADAPTER_DEFAULT,
			D3DDEVTYPE_HAL,
			hWnd,
			D3DCREATE_SOFTWARE_VERTEXPROCESSING,
			&d3dpp,
			&g_pD3DDevice)))
		{
			// 上記の設定が失敗したら
			// 描画と頂点処理をCPUで行なう
			if (FAILED(g_pD3D->CreateDevice(D3DADAPTER_DEFAULT,
				D3DDEVTYPE_REF,
				hWnd,
				D3DCREATE_SOFTWARE_VERTEXPROCESSING,
				&d3dpp,
				&g_pD3DDevice)))
			{
				// 初期化失敗
				return E_FAIL;
			}
		}
	}

	g_d3dpp = d3dpp;

	D3DXCreateFont(g_pD3DDevice, 18, 0, 0, 0, FALSE, SHIFTJIS_CHARSET,
		OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH, _T("Terminal"), &g_pD3DXFont);

	return S_OK;
}

//=============================================================================
// グラフィックの終了処理
//=============================================================================
void UninitDirect3D()
{
	SafeRelease(g_pD3DDevice);
	SafeRelease(g_pD3D);
}


LPDIRECT3DDEVICE9 GetDevice()
{
	return g_pD3DDevice;
}

bool ResetDevice(bool windowMode)
{
	D3DPRESENT_PARAMETERS d3dpp = g_d3dpp;

	D3D_SetWindowMode(&d3dpp, windowMode);

	g_pD3DXFont->OnLostDevice();

	HRESULT hr = g_pD3DDevice->Reset(&d3dpp);

	g_pD3DXFont->OnResetDevice();

	if (FAILED(hr))
	{
		MessageBox(GetHWnd(), DXGetErrorDescription(hr), DXGetErrorString(hr), MB_OK | MB_ICONWARNING);
		hr = g_pD3DDevice->TestCooperativeLevel();
		MessageBox(GetHWnd(), DXGetErrorDescription(hr), DXGetErrorString(hr), MB_OK | MB_ICONWARNING);
		return false;
	}

	g_d3dpp = d3dpp;

	return true;
}

LPD3DXFONT GetFont()
{
	return g_pD3DXFont;
}

void D3D_SetWindowMode(D3DPRESENT_PARAMETERS *d3dpp, bool windowMode)
{
	d3dpp->Windowed = windowMode;

	// ウィンドウモード
	if (windowMode)
	{
		d3dpp->BackBufferFormat = D3DFMT_UNKNOWN;						// バックバッファ
		d3dpp->FullScreen_RefreshRateInHz = 0;							// リフレッシュレート
		d3dpp->PresentationInterval = D3DPRESENT_INTERVAL_IMMEDIATE;	// インターバル
	}
	// フルスクリーンモード
	else
	{
		d3dpp->BackBufferFormat = D3DFMT_X8R8G8B8;						// バックバッファ
		d3dpp->FullScreen_RefreshRateInHz = D3DPRESENT_RATE_DEFAULT;	// リフレッシュレート
		d3dpp->PresentationInterval = D3DPRESENT_INTERVAL_DEFAULT;		// インターバル
	}

}
