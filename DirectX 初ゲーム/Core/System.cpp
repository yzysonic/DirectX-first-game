#include "System.h"
#include "Direct3D.h"
#include "Window.h"
#include "Input.h"
#include "Sound.h"


//=============================================================================
// システムの初期化処理
//=============================================================================
HRESULT InitSystem()
{
	HRESULT hr;

	// ウィンドウの初期化
	hr = Window::Init();

	// グラフィックスの初期化
	if (SUCCEEDED(hr))
		hr = Direct3D::Init(Window::GetHWnd(), Window::GetWindowMode());

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