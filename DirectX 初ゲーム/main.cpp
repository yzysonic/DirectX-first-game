//=============================================================================
//
// �t�@�C�����Fmain.cpp
// ��ҁFGP11A143 38 �k�q�B
//
//=============================================================================
#include "Core/System.h"
#include "Core/Game.h"
#include "Core/GameManager.h"
#include "SceneGlobal.h"
#include "SceneTitle.h"
#ifdef _DEBUG
#include "SceneGame.h"
#include "SceneTest.h"
#endif

// ���������[�N�̎������o
#ifdef _DEBUG
#include <crtdbg.h>
#define new new(_NORMAL_BLOCK, __FILE__, __LINE__)
#endif

//=============================================================================
// ���C���֐�
//=============================================================================
int APIENTRY WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{

#ifdef _DEBUG
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF); // ���������[�N�������o
#endif

	SystemParameters::windowTitle = "�q�b�g�|���S��";

#ifdef _DEBUG
	SystemParameters::FPS = 70;
#endif

	// ������
	if (FAILED(InitSystem()))
		return 1;

	// �}�E�X���B��
	//ShowCursor(false);

	// �Q�[��������
	InitGame();
	GameManager::GetInstance()->SetGlobalScene(new SceneGlobal);
#ifndef _DEBUG
	GameManager::GetInstance()->SetScene(new SceneTitle);
#else
	GameManager::GetInstance()->SetScene(new SceneTest);
#endif

	// �Q�[�����[�v
	RunGame();

	// �I��
	UninitGame();
	UninitSystem();

	return 0;
}
