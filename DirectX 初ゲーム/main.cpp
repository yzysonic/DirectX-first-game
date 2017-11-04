//=============================================================================
//
// �t�@�C�����Fmain.cpp
// ��ҁFGP11A143 38 �k�q�B
//
//=============================================================================
#include "System.h"
#include "Game.h"
#include "GameManager.h"
#include "SceneGlobal.h"
#include "SceneTitle.h"

//=============================================================================
// ���C���֐�
//=============================================================================
int APIENTRY WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{

	// ������
	if (FAILED(InitSystem()))
		return 1;
	
	// �Q�[��������
	InitGame();
	GameManager::GetInstance()->SetGlobalScene(new SceneGlobal);
	GameManager::GetInstance()->SetScene(new SceneTitle);

	// �Q�[�����[�v
	RunGame();

	// �I��
	UninitGame();
	UninitSystem();

	return 0;
}
