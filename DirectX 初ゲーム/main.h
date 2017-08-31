//=============================================================================
//
// ファイル名：main.h
// 作者：GP11A143 38 楊子毅
//
//=============================================================================
#ifndef _MAIN_H_
#define _MAIN_H_

#include <windows.h>
#include <tchar.h>
#include <string.h>
#include <stdio.h>
#include <d3dx9math.h>
//#include <stdio.h>
//#include "mmsystem.h"


#define WINDOW_CLASS_NAME	_T("MainClass")			// ウインドウのクラス名
#define WINDOW_TITLE		_T("DirectX 初ゲーム")	// ウインドウのキャプション名
#define SCREEN_WIDTH		(1280)					// ウインドウの幅
#define SCREEN_HEIGHT		(720)					// ウインドウの高さ
#define SCREEN_CENTER_X		(SCREEN_WIDTH / 2)		// ウインドウの中心Ｘ座標
#define SCREEN_CENTER_Y		(SCREEN_HEIGHT / 2)		// ウインドウの中心Ｙ座標
#define MAX_FPS				(60)					// 最大FPS
#define ObjectMax			(1000)					// 最大オブジェクト数

#define New(obj)			(obj*)malloc(sizeof(obj))
#define SafeDelete(obj)		if(obj != NULL) free(obj)
#define SafeRelease(ptr)	if(ptr != NULL) {ptr->Release(); ptr = NULL; }
#define ColorRGBA(r,g,b,a)		D3DCOLOR_RGBA(r,g,b,a)

//// メモリリーク自動検出
#ifdef _DEBUG
#define _CRTDBG_MAP_ALLOC
#include <crtdbg.h>
#define new new(_NORMAL_BLOCK, __FILE__, __LINE__)
#endif


typedef D3DXVECTOR2 Vector2;
typedef D3DXVECTOR3 Vector3;
typedef D3DCOLOR	Color;

HWND GetHWnd();


#endif