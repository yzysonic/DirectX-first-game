//=============================================================================
//
// ファイル名：graphics.cpp
// 作者：GP11A143 38 楊子毅
//
//=============================================================================
#include "Direct3D.h"
#include "Common.h"
#include "RenderTarget.h"
#include <DxErr.h>

#pragma comment (lib, "d3d9.lib")
#pragma comment (lib, "d3dx9.lib")
#pragma comment (lib, "dxerr.lib")
#pragma comment (lib, "legacy_stdio_definitions.lib")


LPDIRECT3D9				Direct3D::s_pD3D = NULL;
LPDIRECT3DDEVICE9		Direct3D::s_pDevice = NULL;
LPD3DXFONT				Direct3D::s_pFont = NULL;
D3DPRESENT_PARAMETERS	Direct3D::s_d3dpp = {};
HWND					Direct3D::s_hWnd = NULL;


//=============================================================================
// グラフィックの初期化処理
//=============================================================================
HRESULT Direct3D::Init(HWND hWnd, bool bWindowMode)
{
	D3DPRESENT_PARAMETERS d3dpp;
	D3DDISPLAYMODE d3ddm;

	// Direct3Dオブジェクトの作成
	s_pD3D = Direct3DCreate9(D3D_SDK_VERSION);
	if (s_pD3D == NULL)
	{
		return E_FAIL;
	}

	// 現在のディスプレイモードを取得
	if (FAILED(s_pD3D->GetAdapterDisplayMode(D3DADAPTER_DEFAULT, &d3ddm)))
	{
		return E_FAIL;
	}

	// デバイスのプレゼンテーションパラメータの設定
	ZeroMemory(&d3dpp, sizeof(d3dpp));					// ワークをゼロクリア
	d3dpp.BackBufferCount = 1;							// バックバッファの数
	d3dpp.BackBufferWidth = SystemParameters::ResolutionX;	// ゲーム画面サイズ(幅)
	d3dpp.BackBufferHeight = SystemParameters::ResolutionY;	// ゲーム画面サイズ(高さ)
	d3dpp.BackBufferFormat = D3DFMT_UNKNOWN;			// バックバッファのフォーマットは現在設定されているものを使う
	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;			// 映像信号に同期してフリップする
	d3dpp.hDeviceWindow = hWnd;							// 使用するウィンド
	d3dpp.Windowed = bWindowMode;						// ウィンドウモードの設定
	d3dpp.EnableAutoDepthStencil = TRUE;				// デプスバッファ（Ｚバッファ）とステンシルバッファを作成
	d3dpp.AutoDepthStencilFormat = D3DFMT_D16;			// デプスバッファとして16bitを使う
	d3dpp.BackBufferFormat = d3ddm.Format;				// カラーモードの指定
	d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_IMMEDIATE;

	// デバイスの生成
	// ディスプレイアダプタを表すためのデバイスを作成
	// 描画と頂点処理をハードウェアで行なう
	if (FAILED(s_pD3D->CreateDevice(D3DADAPTER_DEFAULT,							// ディスプレイアダプタ
		D3DDEVTYPE_HAL,								// ディスプレイタイプ
		hWnd,										// フォーカスするウインドウへのハンドル
		D3DCREATE_HARDWARE_VERTEXPROCESSING,		// デバイス作成制御の組み合わせ
		&d3dpp,										// デバイスのプレゼンテーションパラメータ
		&s_pDevice)))							// デバイスインターフェースへのポインタ
	{
		// 上記の設定が失敗したら
		// 描画をハードウェアで行い、頂点処理はCPUで行なう
		if (FAILED(s_pD3D->CreateDevice(D3DADAPTER_DEFAULT,
			D3DDEVTYPE_HAL,
			hWnd,
			D3DCREATE_SOFTWARE_VERTEXPROCESSING,
			&d3dpp,
			&s_pDevice)))
		{
			// 上記の設定が失敗したら
			// 描画と頂点処理をCPUで行なう
			if (FAILED(s_pD3D->CreateDevice(D3DADAPTER_DEFAULT,
				D3DDEVTYPE_REF,
				hWnd,
				D3DCREATE_SOFTWARE_VERTEXPROCESSING,
				&d3dpp,
				&s_pDevice)))
			{
				// 初期化失敗
				return E_FAIL;
			}
		}
	}

	s_d3dpp = d3dpp;
	s_hWnd = hWnd;

	D3DXCreateFont(s_pDevice, 18, 0, 0, 0, FALSE, SHIFTJIS_CHARSET,
		OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH, _T("Terminal"), &s_pFont);

	return S_OK;
}

//=============================================================================
// グラフィックの終了処理
//=============================================================================
void Direct3D::Uninit()
{
	SafeRelease(s_pDevice);
	SafeRelease(s_pD3D);
}


LPDIRECT3DDEVICE9 Direct3D::GetDevice()
{
	return s_pDevice;
}

bool Direct3D::ResetDevice()
{

	s_pFont->OnLostDevice();
	RenderTarget::OnLostDevice();

	HRESULT hr = s_pDevice->Reset(&s_d3dpp);

	s_pFont->OnResetDevice();
	RenderTarget::OnResetDevice();

	if (FAILED(hr))
	{
		MessageBox(s_hWnd, DXGetErrorDescription(hr), DXGetErrorString(hr), MB_OK | MB_ICONWARNING);
		hr = s_pDevice->TestCooperativeLevel();
		MessageBox(s_hWnd, DXGetErrorDescription(hr), DXGetErrorString(hr), MB_OK | MB_ICONWARNING);
		return false;
	}

	// レンダリングステートパラメータの設定
	Direct3D::GetDevice()->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);			// カリングを行わない
	Direct3D::GetDevice()->SetRenderState(D3DRS_ZENABLE, TRUE);						// Zバッファを使用
	Direct3D::GetDevice()->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);			// αブレンドを行う
	Direct3D::GetDevice()->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);		// αソースカラーの指定
	Direct3D::GetDevice()->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);	// αデスティネーションカラーの指定

	// サンプラーステートパラメータの設定
	Direct3D::GetDevice()->SetSamplerState(0, D3DSAMP_ADDRESSU, D3DTADDRESS_WRAP);	// テクスチャＵ値の繰り返し設定
	Direct3D::GetDevice()->SetSamplerState(0, D3DSAMP_ADDRESSV, D3DTADDRESS_WRAP);	// テクスチャＶ値の繰り返し設定
	Direct3D::GetDevice()->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);	// テクスチャ拡大時の補間設定
	Direct3D::GetDevice()->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);	// テクスチャ縮小時の補間設定

	// テクスチャステージ加算合成処理
	Direct3D::GetDevice()->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);	// 最初のアルファ引数
	Direct3D::GetDevice()->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);	// アルファブレンディング処理
	Direct3D::GetDevice()->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_DIFFUSE);	// ２番目のアルファ引数


	return true;
}

LPD3DXFONT Direct3D::GetFont()
{
	return s_pFont;
}

bool Direct3D::SetWindowMode(bool windowMode)
{
	s_d3dpp.Windowed = windowMode;

	// ウィンドウモード
	if (windowMode)
	{
		s_d3dpp.BackBufferFormat = D3DFMT_UNKNOWN;						// バックバッファ
		s_d3dpp.FullScreen_RefreshRateInHz = 0;							// リフレッシュレート
		s_d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_IMMEDIATE;	// インターバル
	}
	// フルスクリーンモード
	else
	{
		s_d3dpp.BackBufferFormat = D3DFMT_A8R8G8B8;						// バックバッファ
		s_d3dpp.FullScreen_RefreshRateInHz = D3DPRESENT_RATE_DEFAULT;	// リフレッシュレート
		s_d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_DEFAULT;		// インターバル
	}

	return ResetDevice();
}
