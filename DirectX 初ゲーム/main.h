//=============================================================================
//
// �t�@�C�����Fmain.h
// ��ҁFGP11A143 38 �k�q�B
//
//=============================================================================
#pragma once

#include <windows.h>
#include <tchar.h>
#include <string.h>
#include <stdio.h>
#include <d3dx9math.h>


#define WINDOW_CLASS_NAME	_T("MainClass")			// �E�C���h�E�̃N���X��
#define WINDOW_TITLE		_T("DirectX ���Q�[��")	// �E�C���h�E�̃L���v�V������
#define SCREEN_WIDTH		(1280)					// �E�C���h�E�̕�
#define SCREEN_HEIGHT		(720)					// �E�C���h�E�̍���
#define SCREEN_CENTER_X		(SCREEN_WIDTH / 2)		// �E�C���h�E�̒��S�w���W
#define SCREEN_CENTER_Y		(SCREEN_HEIGHT / 2)		// �E�C���h�E�̒��S�x���W
#define MAX_FPS				(60)					// �ő�FPS
#define ObjectMax			(30000)					// �ő�I�u�W�F�N�g��

template<class T> 
void SafeDelete(T *&ptr)
{
	delete ptr;
	ptr = nullptr;
}

#define SafeRelease(ptr)	if(ptr != NULL) {ptr->Release(); ptr = NULL; }
#define ColorRGBA(r,g,b,a)		D3DCOLOR_RGBA(r,g,b,a)

#define PI D3DX_PI

// ���������[�N�������o
#ifdef _DEBUG
#define _CRTDBG_MAP_ALLOC
#include <crtdbg.h>
#define new new(_NORMAL_BLOCK, __FILE__, __LINE__)
#endif