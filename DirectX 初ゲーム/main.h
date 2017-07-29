//=============================================================================
//
// ファイル名：main.h
// 作者：GP11A143 38 楊子毅
//
//=============================================================================
#ifndef _MAIN_H_
#define _MAIN_H_

#define _CRT_SECURE_NO_WARNINGS			// scanf のwarning防止
#define DIRECTINPUT_VERSION 0x0800		// 警告対処

//#define _BULLET_V2

#include "windows.h"
#include <stdio.h>
#include <tchar.h>
#include "dinput.h"
#include "mmsystem.h"

#pragma comment (lib, "dinput8.lib")
#pragma comment (lib, "winmm.lib")

#define SCREEN_WIDTH	(1280)				// ウインドウの幅
#define SCREEN_HEIGHT	(720)				// ウインドウの高さ
#define SCREEN_CENTER_X	(SCREEN_WIDTH / 2)	// ウインドウの中心Ｘ座標
#define SCREEN_CENTER_Y	(SCREEN_HEIGHT / 2)	// ウインドウの中心Ｙ座標



#endif