#include "Window.h"
#include "Common.h"
#include "Direct3D.h"
#include "Game.h"

HINSTANCE Window::s_hInstance = NULL;
HWND Window::s_hWnd = NULL;
MSG  Window::s_Msg;
bool Window::s_bWindowMode = true;
char* Window::s_ClassName = "MainWindow";

//=============================================================================
// Windowsの初期化処理
//=============================================================================
HRESULT Window::Init()
{
	s_hInstance = GetModuleHandle(NULL);
	int width = SystemParameters::ResolutionX;
	int height = SystemParameters::ResolutionY;

	// ウィンドウクラスを登録する
	WNDCLASSEX	wcex = {};

	wcex.cbSize = sizeof(WNDCLASSEX);
	wcex.style = CS_CLASSDC;
	wcex.lpfnWndProc = WindowProc;
	wcex.hInstance = s_hInstance;
	wcex.lpszClassName = s_ClassName;
	wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)(COLOR_BACKGROUND + 1);

	RegisterClassEx(&wcex);


	// ウィンドウオブジェクトを作成する.
	s_hWnd = CreateWindowEx(
		0,							//拡張スタイル（任意）
		s_ClassName,			//クラス名
		SystemParameters::windowTitle,				//ウィンドウ名（タイトル）
		(WS_OVERLAPPED | WS_MINIMIZEBOX | WS_SYSMENU),		//スタイル
		((GetSystemMetrics(SM_CXSCREEN) - width) / 2),		//横方向の位置
		((GetSystemMetrics(SM_CYSCREEN) - height) / 2),	//縦方向の位置
		width + GetSystemMetrics(SM_CXDLGFRAME) * 2,		//幅
		height + GetSystemMetrics(SM_CXDLGFRAME) * 2 + GetSystemMetrics(SM_CYCAPTION),	//高さ
		NULL,						//親ウィンドウ
		NULL,						//メニュー
		s_hInstance,				//アプリケーションインスタンスのハンドル
		NULL						//ウィンドウ作成データ
	);

	if (s_hWnd == NULL)
		return E_FAIL;

	//ウィンドウをスクリーンに表示する
	ShowWindow(s_hWnd, SW_SHOW);
	UpdateWindow(s_hWnd);

	return S_OK;
}


//=============================================================================
// Windowsの終了処理
//=============================================================================
void Window::Uninit()
{
	// ウィンドウのデストロイド
	//DestroyWindow(g_hWnd);
	s_hWnd = NULL;

	// ウィンドウクラスの登録を解除
	UnregisterClass(s_ClassName, s_hInstance);
	s_hInstance = NULL;
}

//=============================================================================
// プロシージャ
//=============================================================================
LRESULT CALLBACK Window::WindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_CLOSE:
		if (EndGame())
		{
			DestroyWindow(Window::GetHWnd());	// ウィンドウを破棄するよう指示する
		}
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
			SetWindowMode(!s_bWindowMode);
			break;
		}

	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}

	return 0;
}

void Window::CheckMesg(void)
{
	if (PeekMessage(&s_Msg, NULL, 0, 0, PM_REMOVE))
	{
		// PostQuitMessage()が呼ばれたらゲーム終了
		if (s_Msg.message == WM_QUIT)
		{
			StopGame();
			return;
		}
		// メッセージの翻訳とディスパッチ
		else
		{
			TranslateMessage(&s_Msg);
			DispatchMessage(&s_Msg);
		}
	}

}


HWND Window::GetHWnd()
{
	return s_hWnd;
}

HINSTANCE Window::GetHInstance(void)
{
	return s_hInstance;
}

bool Window::GetWindowMode(void)
{
	return s_bWindowMode;
}

void Window::SetWindowMode(bool windowMode)
{
	if (!Direct3D::SetWindowMode(windowMode))
		return;

	if (windowMode)
	{
		//SetWindowLong(s_hWnd, GWL_STYLE, WS_CAPTION | WS_THICKFRAME | WS_OVERLAPPED | WS_MINIMIZEBOX | WS_SYSMENU);
		SetWindowPos(s_hWnd, NULL,
			(GetSystemMetrics(SM_CXSCREEN) - SystemParameters::ResolutionX) / 2,
			(GetSystemMetrics(SM_CYSCREEN) - SystemParameters::ResolutionY) / 2,
			0, 0,
			SWP_NOSIZE | SWP_NOZORDER | SWP_SHOWWINDOW);
	}
	//else
	//{
	//	SetWindowLong(s_hWnd, GWL_STYLE, WS_POPUP | WS_EX_TOPMOST);
	//}


	//ShowWindow(s_hWnd, SW_SHOW);

	s_bWindowMode = windowMode;
}