#include "Game.h"
#include <windows.h>
#include "Direct3D.h"
#include "Input.h"
#include "Time.h"
#include "Random.h"
#include "Texture.h"
#include "GameManager.h"
#include "ObjectManager.h"
#include "Renderer.h"

void CheckWinMesg();

bool g_bRunGame = true;
MSG g_Msg;

void InitGame(void)
{
	InitTime();
	InitRandom();
	InitTexture();
	InitObjectManager();
	InitGameManager();
}

void RunGame(void)
{
	while (g_bRunGame)
	{
		CheckWinMesg();
		UpdateInput();
		UpdateGameManager();
		//TODO: UpdatePhysics();
		UpdateObjectManager();
		DrawFrame();
		FramerateControl();
	}
}

void UninitGame(void)
{
	UninitGameManager();
	UninitObjectManager();
	UninitTexture();
	UninitTime();
}

void StopGame(void)
{
	g_bRunGame = false;
}

void CheckWinMesg()
{
	if (PeekMessage(&g_Msg, NULL, 0, 0, PM_REMOVE))
	{
		// PostQuitMessage()���Ă΂ꂽ�烋�[�v�I��
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

