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

// ���������[�N�̎������o
#ifdef _DEBUG
#define _CRTDBG_MAP_ALLOC
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

	// ������
	if (FAILED(InitSystem()))
		return 1;

	// �}�E�X���B��
	//ShowCursor(false);

	// �Q�[��������
	InitGame();
	SceneGlobal::Create();
	GameManager::GetInstance()->SetGlobalScene(SceneGlobal::GetInstance());
	GameManager::GetInstance()->SetScene(new SceneTitle);

	// �Q�[�����[�v
	RunGame();

	// �I��
	UninitGame();
	UninitSystem();

	return 0;
}
