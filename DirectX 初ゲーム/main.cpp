//=============================================================================
//
// ファイル名：main.cpp
// 作者：GP11A143 38 楊子毅
//
//=============================================================================
#include "main.h"
#include "Direct3D.h"
#include "Input.h"
#include "Sound.h"
#include "Game.h"



//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
LRESULT CALLBACK WindowProc(HWND, UINT, WPARAM, LPARAM);
HRESULT InitWindows();
void UninitWindows();
HRESULT InitSystem();
void UninitSystem();


//*****************************************************************************
// グローバル変数:
//*****************************************************************************
HINSTANCE	g_hInstance = NULL;
HWND		g_hWnd = NULL;
bool		g_bWindowMode = true;


//=============================================================================
// メイン関数
//=============================================================================
int APIENTRY WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{

	// 初期化
	if (FAILED(InitSystem()))
		return 1;
	InitGame();

	// ゲームループ
	RunGame();

	// 終了
	UninitGame();
	UninitSystem();

	return 0;
}


//=============================================================================
// プロシージャ
//=============================================================================
LRESULT CALLBACK WindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_CLOSE:
		StopGame();
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;

	case WM_KEYDOWN:
		switch (wParam)
		{
		case VK_ESCAPE:		// [ESC]キーが押された
			StopGame();
			break;
		case VK_F9:			// [F9]キーが押された
			SetWindowMode(!g_bWindowMode);
			break;
		}

	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}

	return 0;
}

//=============================================================================
// Windowsの初期化処理
//=============================================================================
HRESULT InitWindows()
{
	g_hInstance = GetModuleHandle(NULL);

	// ウィンドウクラスを登録する
	WNDCLASSEX	wcex = {};

	wcex.cbSize			= sizeof(WNDCLASSEX);
	wcex.style			= CS_CLASSDC;
	wcex.lpfnWndProc	= WindowProc;
	wcex.hInstance		= g_hInstance;
	wcex.lpszClassName	= WINDOW_CLASS_NAME;
	wcex.hCursor		= LoadCursor(NULL, IDC_ARROW);
	//wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);

	RegisterClassEx(&wcex);


	// ウィンドウオブジェクトを作成する.
	g_hWnd = CreateWindowEx(
		0,							//拡張スタイル（任意）
		WINDOW_CLASS_NAME,			//クラス名
		WINDOW_TITLE,				//ウィンドウ名（タイトル）
		(g_bWindowMode ? (WS_OVERLAPPED | WS_MINIMIZEBOX | WS_SYSMENU) : WS_POPUP),		//スタイル
		(g_bWindowMode ? ((GetSystemMetrics(SM_CXSCREEN) - SCREEN_WIDTH) / 2) : 0),		//横方向の位置
		(g_bWindowMode ? ((GetSystemMetrics(SM_CYSCREEN) - SCREEN_HEIGHT) / 2) : 0),	//縦方向の位置
		SCREEN_WIDTH + GetSystemMetrics(SM_CXDLGFRAME) * 2,		//幅
		SCREEN_HEIGHT + GetSystemMetrics(SM_CXDLGFRAME) * 2 + GetSystemMetrics(SM_CYCAPTION),	//高さ
		NULL,						//親ウィンドウ
		NULL,						//メニュー
		g_hInstance,				//アプリケーションインスタンスのハンドル
		NULL						//ウィンドウ作成データ
	);

	if (g_hWnd == NULL)
		return E_FAIL;

	//ウィンドウをスクリーンに表示する
	ShowWindow(g_hWnd, SW_SHOW);
	UpdateWindow(g_hWnd);

	return S_OK;
}

//=============================================================================
// Windowsの終了処理
//=============================================================================
void UninitWindows()
{
	// ウィンドウのデストロイド
	//DestroyWindow(g_hWnd);
	g_hWnd = NULL;

	// ウィンドウクラスの登録を解除
	UnregisterClass(WINDOW_CLASS_NAME, g_hInstance);
	g_hInstance = NULL;
}

//=============================================================================
// システムの初期化処理
//=============================================================================
HRESULT InitSystem()
{

#ifdef _DEBUG
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF); // メモリリーク自動検出
#endif

	HRESULT hr;

	// ウィンドウズの初期化
	hr = InitWindows();
	
	// グラフィックスの初期化
	if (SUCCEEDED(hr))
		hr = InitDirect3D(g_hWnd, SCREEN_WIDTH, SCREEN_HEIGHT, g_bWindowMode);

	// インプットの初期化
	if (SUCCEEDED(hr))
		hr = InitInput(g_hInstance, g_hWnd);

	// サウンドの初期化
	if (SUCCEEDED(hr))
		hr = InitSound(g_hWnd);

	return hr;
}

//=============================================================================
// システムの終了処理
//=============================================================================
void UninitSystem()
{
	UninitSound();
	UninitInput();
	UninitDirect3D();
	UninitWindows();
}


HWND GetHWnd()
{
	return g_hWnd;
}

bool GetWindowMode(void)
{
	return g_bWindowMode;
}

void SetWindowMode(bool windowMode)
{
	if (!ResetDevice(windowMode))
		return;

	if (windowMode)
	{
		SetWindowLong(g_hWnd, GWL_STYLE, WS_CAPTION | WS_THICKFRAME | WS_OVERLAPPED | WS_MINIMIZEBOX | WS_SYSMENU);
		SetWindowPos(g_hWnd, NULL, 
			(GetSystemMetrics(SM_CXSCREEN) - SCREEN_WIDTH) / 2,
			(GetSystemMetrics(SM_CYSCREEN) - SCREEN_HEIGHT) / 2,
			0, 0,
			SWP_NOSIZE | SWP_NOZORDER | SWP_SHOWWINDOW);
	}
	else
	{
		SetWindowLong(g_hWnd, GWL_STYLE, WS_POPUP | WS_EX_TOPMOST);
	}


	ShowWindow(g_hWnd, SW_SHOW);

	g_bWindowMode = windowMode;
}
