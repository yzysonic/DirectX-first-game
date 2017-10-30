//=============================================================================
//
// ファイル名：main.cpp
// 作者：GP11A143 38 楊子毅
//
//=============================================================================
#include "CoreBase.h"
#include "SceneTitle.h"
#include "GlobalUpdate.h"


//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT InitSystem();
void UninitSystem();


//=============================================================================
// メイン関数
//=============================================================================
int APIENTRY WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{

	// 初期化
	if (FAILED(InitSystem()))
		return 1;
	InitGame(new SceneTitle);
	SetGlobalUpdate(&GlobalUpdate);

	// ゲームループ
	RunGame();

	// 終了
	UninitGame();
	UninitSystem();

	return 0;
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

	// ウィンドウの初期化
	hr = Window::Init();
	
	// グラフィックスの初期化
	if (SUCCEEDED(hr))
		hr = Direct3D::Init(Window::GetHWnd(), SCREEN_WIDTH, SCREEN_HEIGHT, Window::GetWindowMode());

	// インプットの初期化
	if (SUCCEEDED(hr))
		hr = InitInput(Window::GetHInstance(), Window::GetHWnd());

	// サウンドの初期化
	if (SUCCEEDED(hr))
		hr = InitSound(Window::GetHWnd());

	return hr;
}

//=============================================================================
// システムの終了処理
//=============================================================================
void UninitSystem()
{
	UninitSound();
	UninitInput();
	Direct3D::Uninit();
	Window::Uninit();
}