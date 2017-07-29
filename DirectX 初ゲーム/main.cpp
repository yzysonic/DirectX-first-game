//=============================================================================
//
// ファイル名：main.cpp
// 作者：GP11A143 38 楊子毅
//
//=============================================================================
#include "main.h"
#include "graphics.h"


//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define WINDOW_CLASS_NAME		_T("MainClass")			// ウインドウのクラス名
#define WINDOW_TITLE			_T("DirectX 初ゲーム")	// ウインドウのキャプション名

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
LRESULT CALLBACK WindowProc(HWND, UINT, WPARAM, LPARAM);
HRESULT InitWindows();
void UninitWindows();
HRESULT InitSystem();
void UninitSystem();
void UpdateFrame();


//*****************************************************************************
// グローバル変数:
//*****************************************************************************
HINSTANCE g_hInstance = NULL;
HWND g_hWnd = NULL;

#ifdef _DEBUG
LPD3DXFONT	g_pD3DXFont = NULL;	// フォントへのポインタ
int			g_nCountFPS;		// FPSカウンタ
#endif


//=============================================================================
// メイン関数
//=============================================================================
int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);	// 無くても良いけど、警告が出る（未使用宣言）
	UNREFERENCED_PARAMETER(lpCmdLine);		// 無くても良いけど、警告が出る（未使用宣言）

	// 時間計測用
	DWORD dwExecLastTime = timeGetTime();
	DWORD dwFPSLastTime = timeGetTime();
	DWORD dwCurrentTime = 0;
	DWORD dwFrameCount = 0;
	timeBeginPeriod(1); // 分解能を設定

	//システム初期化
	HRESULT hr;
	hr = InitSystem();

	if (FAILED(hr))
		return (-1);

	// メッセージループ
	MSG msg;
	while(true)
	{
		if(PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			if (msg.message == WM_QUIT)
			{
				// PostQuitMessage()が呼ばれたらループ終了
				break;
			}
			else
			{
				// メッセージの翻訳とディスパッチ
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
		}


		dwCurrentTime = timeGetTime();

		if ((dwCurrentTime - dwFPSLastTime) >= 500) // 0.5秒ごとに実行
		{
#ifdef _DEBUG
			g_nCountFPS = (dwFrameCount * 1000) / (dwCurrentTime - dwFPSLastTime); // FPSを計測
#endif
			dwFPSLastTime = dwCurrentTime;	//　FPS計測時刻を保存
			dwFrameCount = 0;				// カウントをクリア
		}

		if ((dwCurrentTime - dwExecLastTime) >= (1000/60)) // 1/60秒ごとに実行
		{
			dwExecLastTime = dwCurrentTime; // 処理した時刻を保存

			UpdateFrame();	// 更新処理
			DrawFrame();	// 描画処理

			dwFrameCount++; //処理回数のカウントを加算
		}

	}
	
	// システム終了
	UninitSystem();

	return (int)msg.wParam;
}


//=============================================================================
// プロシージャ
//=============================================================================
LRESULT CALLBACK WindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_DESTROY:
		PostQuitMessage(0);
		break;

	case WM_KEYDOWN:
		switch (wParam)
		{
		case VK_ESCAPE:					// [ESC]キーが押された
			DestroyWindow(hWnd);		// ウィンドウを破棄するよう指示する
			break;
		}
		break;

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

	wcex.cbSize = sizeof(WNDCLASSEX);
	wcex.style = CS_CLASSDC;
	wcex.lpfnWndProc = WindowProc;
	wcex.hInstance = g_hInstance;
	wcex.lpszClassName = WINDOW_CLASS_NAME;
	//wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
	//wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);

	RegisterClassEx(&wcex);


	// ウィンドウオブジェクトを作成する.
	g_hWnd = CreateWindowEx(
		0,							//拡張スタイル（任意）
		WINDOW_CLASS_NAME,			//クラス名
		WINDOW_TITLE,				//ウィンドウ名（タイトル）
		WS_OVERLAPPED | WS_MINIMIZEBOX | WS_SYSMENU,			//スタイル
		(GetSystemMetrics(SM_CXSCREEN) - SCREEN_WIDTH) / 2,		//横方向の位置
		(GetSystemMetrics(SM_CYSCREEN) - SCREEN_HEIGHT) / 2,	//縦方向の位置
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
	DestroyWindow(g_hWnd);
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
	HRESULT hr;

	// ウィンドウズの初期化
	hr = InitWindows();
	
	// グラフィックスの初期化
	if (SUCCEEDED(hr))
		hr = InitGraphics(g_hWnd, SCREEN_WIDTH, SCREEN_HEIGHT, true);

	return hr;
}

//=============================================================================
// システムの終了処理
//=============================================================================
void UninitSystem()
{
	UninitGraphics();
	UninitWindows();
}

//=============================================================================
// 更新処理処理
//=============================================================================
void UpdateFrame()
{
	DrawFrame();
}