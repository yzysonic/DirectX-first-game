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

//=============================================================================
// ���C���֐�
//=============================================================================
int APIENTRY WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{

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
