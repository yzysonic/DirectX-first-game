//=============================================================================
//
// ファイル名：common.h
// 作者：GP11A143 38 楊子毅
//
//=============================================================================
#pragma once

#include <windows.h>
#include <tchar.h>
#include <string.h>
#include <stdio.h>

//システムのパラメータ
//数値の変更に反映するのはシステム初期化するときのみ。
struct SystemParameters
{
	static char* windowTitle;	// ウィンドウタイトル
	static int ResolutionX;		// 画面の解像度X
	static int ResolutionY;		// 画面の解像度Y
	static int FPS;				// 最大FPS
	static int SystemParameters::ObjectMax;		// 最大オブジェクト数
};

template<class T>
void SafeDelete(T *&ptr)
{
	delete ptr;
	ptr = nullptr;
}

#define SafeRelease(ptr)	if(ptr != NULL) {ptr->Release(); ptr = NULL; }
#define ColorRGBA(r,g,b,a)		D3DCOLOR_RGBA(r,g,b,a)

#define PI ((FLOAT)  3.141592654f)

// メモリリークの自動検出

#ifdef _DEBUG
//#define _CRTDBG_MAP_ALLOC
//#include <crtdbg.h>
//#define new new(_NORMAL_BLOCK, __FILE__, __LINE__)
#endif