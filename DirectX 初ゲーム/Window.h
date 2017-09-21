#pragma once

#include <Windows.h>

class Window
{
public:
	static HRESULT Init(void);
	static void Uninit(void);
	static LRESULT CALLBACK WindowProc(HWND, UINT, WPARAM, LPARAM);
	static void CheckMesg(void);
	static HWND GetHWnd(void);
	static HINSTANCE GetHInstance(void);
	static bool GetWindowMode(void);
	static void SetWindowMode(bool windowMode);

private:
	static HINSTANCE s_hInstance;
	static HWND	s_hWnd;
	static MSG s_Msg;
	static bool	s_bWindowMode;
};