#include "Game.h"
#include <windows.h>
#include "Direct3D.h"
#include "Input.h"
#include "Time.h"
#include "Texture.h"
#include "ObjectManager.h"
#include "Renderer.h"
#include "Physics.h"
#include "GameManager.h"

// �v���g�^�C�v�錾
void CheckWinMesg();

// �O���[�o���ϐ�
bool g_bRunGame = true;
MSG g_Msg;


// �Q�[��������
void InitGame(void)
{
	Time::Init();
	InitTexture();
	ObjectManager::Create();
	Renderer::Create();
	Physics::Create();
	GameManager::Create();
}

// �Q�[�����[�v
void RunGame(void)
{
	Renderer* renderer			= Renderer::GetInstance();
	Physics* physics			= Physics::GetInstance();
	ObjectManager* objManager	= ObjectManager::GetInstance();

	while (g_bRunGame)
	{
		CheckWinMesg();
		UpdateInput();
		GameManager::Update();
		objManager->update();
		physics->update();
		renderer->drawFrame();
		Time::FramerateControl();
	}
}

// �Q�[���I������
void UninitGame(void)
{
	GameManager::Destroy();
	Physics::Destroy();
	Renderer::Destroy();
	ObjectManager::Destroy();
	UninitTexture();
	Time::Uninit();
}

// �Q�[���I��
void StopGame(void)
{
	DestroyWindow(GetHWnd());	// �E�B���h�E��j������悤�w������
}

// �E�B���h�E�Y���b�Z�[�W����
void CheckWinMesg()
{
	if (PeekMessage(&g_Msg, NULL, 0, 0, PM_REMOVE))
	{
		// PostQuitMessage()���Ă΂ꂽ��Q�[���I��
		if (g_Msg.message == WM_QUIT)
		{
			g_bRunGame = false;
			return;
		}
		// ���b�Z�[�W�̖|��ƃf�B�X�p�b�`
		else
		{
			TranslateMessage(&g_Msg);
			DispatchMessage(&g_Msg);
		}
	}
}

