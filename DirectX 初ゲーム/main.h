//=============================================================================
//
// �t�@�C�����Fmain.h
// ��ҁFGP11A143 38 �k�q�B
//
//=============================================================================
#ifndef _MAIN_H_
#define _MAIN_H_

#define _CRT_SECURE_NO_WARNINGS			// scanf ��warning�h�~
#define DIRECTINPUT_VERSION 0x0800		// �x���Ώ�

//#define _BULLET_V2

#include "windows.h"
#include <stdio.h>
#include <tchar.h>
#include "dinput.h"
#include "mmsystem.h"

#pragma comment (lib, "dinput8.lib")
#pragma comment (lib, "winmm.lib")

#define SCREEN_WIDTH	(1280)				// �E�C���h�E�̕�
#define SCREEN_HEIGHT	(720)				// �E�C���h�E�̍���
#define SCREEN_CENTER_X	(SCREEN_WIDTH / 2)	// �E�C���h�E�̒��S�w���W
#define SCREEN_CENTER_Y	(SCREEN_HEIGHT / 2)	// �E�C���h�E�̒��S�x���W



#endif